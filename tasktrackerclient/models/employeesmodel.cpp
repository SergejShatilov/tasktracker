
#include "employeesmodel.h"

//#include "tasksmodel.h"
//#include <QBrush>

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
/*void EmployeesModel::setTasksModel(QAbstractItemModel* model) {
    m_tasksModel = model;
}

// =============================================================================
void EmployeesModel::addEmployee(const Employee& employee)
{
    auto parentIndex = QModelIndex();

    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));

    EmployeeObject* employeeObject = new EmployeeObject(employee);
    m_listObjects << employeeObject;

    employeeObject->setParent(m_rootItem);

    endInsertRows();
}

// =============================================================================
void EmployeesModel::removeEmployee(const QModelIndex& index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row() + 1);

    auto obj = employeeObjectByIndex(index);

    m_listObjects.removeOne(obj);
    delete obj;

    endRemoveRows();
}

// =============================================================================
void EmployeesModel::loadFromList(const QList<Employee>& list)
{
    clear();

    for (const auto& employee : list) {
        addEmployee(employee);
    }
}

// =============================================================================
qint32 EmployeesModel::idByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return -1;

    EmployeeObject* employeeObject = employeeObjectByIndex(index);
    return employeeObject->id();
}

// =============================================================================
qint32 EmployeesModel::idByFullName(const QString& fullName) const
{
    const auto& children = m_rootItem->children();
    auto it = std::find_if
    (
        children.cbegin(),
        children.cend(),
        [&fullName](const QObject* obj) {
            return (obj->property("fullName").toString() == fullName);
        }
    );

    if (it == children.cend())
        return 0;

    return (*it)->property("id").toInt();
}
*/

// =============================================================================
QVariant EmployeesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::DisplayRole:
        {
            const auto& field = fieldByColumn(index.column());
            auto employee = static_cast<Employee*>(objectByIndex(index));

            // Если поле с задачами с истекшим временем выполнения
            if (field == "expiredTasks")
            {
                return 0;
            }

            return employee->property(field.toLocal8Bit());
        }
        default:
            return QVariant();
    }


    /*

    switch (role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            const auto& propertyName = m_columns.at(index.column());
            const auto obj = employeeObjectByIndex(index);

            // Если колонка с задачами
            if (propertyName == "tasks" && m_tasksModel != nullptr)
            {
                auto executorId = obj->id();
                const auto listTasks = static_cast<TasksModel*>(m_tasksModel)->findByExecutorId(executorId);

                int count = 0;

                for (const auto taskObject : listTasks)
                {
                    if (taskObject->state() == Task::State::Completed)
                        continue;

                    if (taskObject->deadline() >= QDate::currentDate())
                        continue;

                    count++;
                }

                return QVariant(count);
            }

            return obj->property(propertyName.toUtf8());
        }
        case Qt::BackgroundRole :
        {
            const auto& propertyName = m_columns.at(index.column());

            // Если колонка с задачами
            if (propertyName == "tasks" && m_tasksModel != nullptr)
            {
                auto count = data(index, Qt::DisplayRole);
                if (count > 0)
                    return QBrush(Qt::red);
            }

            return QVariant();
        }
        default:
            return QVariant();
    }*/
    return QVariant();
}

// =============================================================================
Qt::ItemFlags EmployeesModel::flags(const QModelIndex &index) const
{
    /*if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == 8)
        flags |= Qt::ItemIsEditable;

    return flags;*/
    return ObjectsModel::flags(index);
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

/*
// =============================================================================
QString EmployeesModel::fullNameById(qint32 id) const
{
    if (id == 0)
        return QString(tr("Not assigned"));

    auto it = std::find_if
    (
        m_listObjects.cbegin(),
        m_listObjects.cend(),
        [id](const EmployeeObject* obj) {
            return (obj->id() == id);
        }
    );

    if (it == m_listObjects.cend())
        return QString::number(id);

    return (*it)->fullName();
}*/
// =============================================================================
