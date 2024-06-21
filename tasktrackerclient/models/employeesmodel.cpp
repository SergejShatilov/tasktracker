
#include "employeesmodel.h"

#include <QBrush>

// =============================================================================
EmployeesModel::EmployeesModel(DbRemoteManager* dbManager, QObject* parent) :
    ObjectsModel(dbManager, parent)
{
    addField("id",           tr("ID"),            false);
    addField("lastName",     tr("Last Name"),     false);
    addField("firstName",    tr("First Name"),    false);
    addField("patronymic",   tr("Patronymic"),    false);
    addField("fullName",     tr("Full Name"),     true);
    addField("position",     tr("Position"),      true);
    addField("email",        tr("Email"),         true);
    addField("phone",        tr("Phone"),         true);
    addField("expiredTasks", tr("Expired Tasks"), true);

    connect(dbManager, &DbRemoteManager::addedEmployee,
            this, &EmployeesModel::addEmployee, Qt::DirectConnection);

    connect(dbManager, &DbRemoteManager::removedEmployee,
            this, &EmployeesModel::removeEmployee, Qt::DirectConnection);

    connect(dbManager, &DbRemoteManager::updateEmployees,
            this, &EmployeesModel::updateEmployees, Qt::DirectConnection);
}

// =============================================================================
QVariant EmployeesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& field = fieldByColumn(index.column());
    auto employee = static_cast<Employee*>(objectByIndex(index));

    switch (role)
    {
        case Qt::DisplayRole:
        {
            // Если поле с задачами с истекшим временем выполнения
            if (field == "expiredTasks")
            {
                // Отображаем кол-во задач с истекшим временем исполнения
                int expiredTasksCount = 0;

                for (auto obj : employee->tasks())
                {
                    auto task = static_cast<Task*>(obj);
                    if ((task->state() != Task::State::Completed) &&
                        (task->deadline() < QDate::currentDate()))
                    {
                         ++(expiredTasksCount);
                    }
                }

                return expiredTasksCount;
            }

            return employee->property(field.toLocal8Bit());
        }
        case Qt::EditRole:
        {
            // Для просроченных задач выдаем id исполнителя
            if (field == "expiredTasks")
                return employee->id();

            return QVariant();
        }
        case Qt::BackgroundRole:
        {
            // Если поле с задачами с истекшим временем выполнения
            if (field == "expiredTasks")
            {
                int columnExpiredTasks = columnByField(field);
                auto indexExpiredTasks = this->index(
                    index.row(), columnExpiredTasks, index.parent()
                );

                if (data(indexExpiredTasks, Qt::DisplayRole).toInt() > 0)
                    return QBrush(QColor(200, 120, 120));

            }
        }
        default:
            return QVariant();
    }

    return QVariant();
}

// =============================================================================
Qt::ItemFlags EmployeesModel::flags(const QModelIndex &index) const
{
    auto flags = ObjectsModel::flags(index);

    if (!index.isValid())
        return flags;

    // Колонку с просрочеными задачами делаем редактируемой,
    // чтобы была возможность вызвать делегат
    const auto& field = fieldByColumn(index.column());
    if (field == "expiredTasks")
    {
        // Если просроченных задач нет, то делегат запрещаем вызывать делегат
        if (index.data().toInt() != 0)
            flags |= Qt::ItemIsEditable;
    }

    return flags;
}

// =============================================================================
void EmployeesModel::addEmployee(Employee* employee)
{
    const QModelIndex parentIndex = QModelIndex();

    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));

    employee->setParent(m_rootObject);

    endInsertRows();
}

// =============================================================================
void EmployeesModel::removeEmployee(Employee* employee)
{
    const QModelIndex index = indexOf(employee);

    if (!index.isValid())
        return;

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    delete employee;

    endRemoveRows();
}

// =============================================================================
void EmployeesModel::updateEmployees(const QList<Employee*>& listEmployees)
{
    // Удаляем все объекты
    reset();

    // Добавляем объекты в модель
    const QModelIndex parentIndex = QModelIndex();
    beginInsertRows(parentIndex, rowCount(parentIndex),
                    rowCount(parentIndex) + listEmployees.size());

    for (auto employee : listEmployees)
        employee->setParent(m_rootObject);

    endInsertRows();
}

// =============================================================================
