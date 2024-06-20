
#include "tasksmodel.h"

#include <QBrush>
#include <QDebug>

// =============================================================================
TasksModel::TasksModel(DbRemoteManager* dbManager, QObject* parent) :
    ObjectsModel(dbManager, parent)
{
    addField("name",            tr("Name"),         true);
    addField("id",              tr("ID"),           true);
    addField("stateIndex",      tr("State Index"),  false);
    addField("stateString",     tr("State String"), false);
    addField("stateDisplay",    tr("State"),        true);
    addField("executorId",      tr("Executor ID"),  false);
    addField("executor",        tr("Executor"),     true);
    addField("deadline",        tr("Deadline"),     true);
    addField("parentId",        tr("Parent ID"),    false);
    addField("description",     tr("Description"),  true);

    connect(dbManager, &DbRemoteManager::addedTask,
            this, &TasksModel::addTask, Qt::DirectConnection);

    connect(dbManager, &DbRemoteManager::removedTask,
            this, &TasksModel::removeTask, Qt::DirectConnection);

    connect(dbManager, &DbRemoteManager::updateTasks,
            this, &TasksModel::updateTasks, Qt::DirectConnection);
}

// =============================================================================
QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& field = fieldByColumn(index.column());
    auto task = static_cast<Task*>(objectByIndex(index));
    auto executor = static_cast<Employee*>(task->executor());

    switch (role)
    {
        case Qt::DisplayRole:
        {
            // Если отображаемое состояние задачи
            if (field == "stateDisplay")
            {
                switch(task->state())
                {
                    case Task::State::NotStarted:
                        return QString(tr("Not Started"));
                    case Task::State::Work:
                        return QString(tr("Work"));
                    case Task::State::Suspended:
                        return QString(tr("Suspended"));
                    case Task::State::Completed:
                        return QString(tr("Completed"));
                    default:
                        return QString(tr("<Unknown>"));
                }
            }

            // Если отображаемые ФИО исполнителя
            if (field == "executor")
            {
                // Если исполнитель не назначен, то отображаем "<Не выбрано>"
                if (executor == nullptr)
                    return QString(tr("<Not Selected>"));

                return executor->fullName();
            }

            return task->property(field.toLocal8Bit());
        }
        case Qt::EditRole:
        {
            // Для отображаемого состояния возвращаем индекс этого состояния
            if (field == "stateDisplay")
                return task->stateIndex();

            return QVariant();
        }
        default:
            return QVariant();
    }

    /*switch (role)
    {

        case Qt::BackgroundRole:
        {
            const auto& propertyName = m_columns.at(index.column());
            const auto obj = taskObjectByIndex(index);

            // Если колонка с состоянием
            if (propertyName == "state")
            {
                // Если задача выполнена, то отображаем ее зеленой,
                // независимо от дедлайна
                if (obj->state() == Task::State::Completed)
                    return QBrush(Qt::green);

                // Если задача просрочена, то отображаем поле
                // состояние красным
                if (obj->deadline() < QDate::currentDate())
                    return QBrush(Qt::red);

                switch (obj->state())
                {
                    case Task::State::NotStarted:
                    case Task::State::Suspended:
                        return QBrush(Qt::lightGray);
                    default:
                        return QVariant();
                }
            }

            return QVariant();
        }
        case Qt::ToolTipRole:
        {
            const auto& propertyName = m_columns.at(index.column());
            const auto obj = taskObjectByIndex(index);

            // Если колонка с исполнителем
            if (propertyName == "executorId" && m_employeesModel != nullptr)
            {
                return obj->executorId();
            }

            // Если колонка с состоянием
            else if (propertyName == "state" && m_employeesModel != nullptr)
            {
                return obj->stateString();
            }

            // Если колонка с датой
            else if (propertyName == "deadline" && m_employeesModel != nullptr)
            {
                return obj->deadline();
            }

            return QVariant();
        }
        default:
            return QVariant();
    }*/

    return QVariant();
}

// =============================================================================
bool TasksModel::setData(const QModelIndex &index,
                         const QVariant &value,
                         int role)
{
    if (!index.isValid())
        return false;

    const auto& field = fieldByColumn(index.column());
    auto task = static_cast<Task*>(objectByIndex(index));

    switch (role)
    {
        case Qt::EditRole:
        {
            // Если колонка с состоянием
            if (field == "stateDisplay")
            {
                task->setStateIndex(value.toInt());
                return m_dbManager->changeTask(task);
            }

            return false;
        }
        default:
            return false;
    }
}

// =============================================================================
Qt::ItemFlags TasksModel::flags(const QModelIndex& index) const
{
    auto flags = ObjectsModel::flags(index);

    if (!index.isValid())
        return flags;

    // Колонку с отображаемым состоянием задачи можно редактировать
    const auto& field = fieldByColumn(index.column());
    if (field == "stateDisplay")
        flags |= Qt::ItemIsEditable;

    return flags;
}

// =============================================================================
void TasksModel::addTask(Task* task)
{
    QObject* parent = task->parent();

    //qDebug() << parent;

    // Если родитель не найден во всей модели,
    // то родителем назначаем корневой объект
    if (!isChild(parent))
        parent = m_rootObject;

    const QModelIndex parentIndex = indexOf(parent);

    //qDebug() << parent << parentIndex << m_rootObject;

    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));

    task->setParent(parent);

    endInsertRows();
}

// =============================================================================
void TasksModel::removeTask(Task* task, bool isExtract)
{
    const QModelIndex index = indexOf(task);

    if (!index.isValid())
        return;

    beginRemoveRows(index.parent(), index.row(), index.row());

    // Если необходимо все подзадачи перенести наружу
    if (isExtract)
    {
        // Пробегаемся по всем дочерним элементам
        // и устанавливаем в качестве родителя пропродителя
        auto parent = task->parent();
        for (auto child : task->children())
            child->setParent(parent);
    }

    delete task;

    endRemoveRows();
}

// =============================================================================
void TasksModel::updateTasks(QList<Task*> listTasks)
{
    // Удаляем все объекты
    reset();

    // Добавляем объекты (только верхнего уровня) в модель
    const QModelIndex parentIndex = QModelIndex();
    beginInsertRows(parentIndex, rowCount(parentIndex),
                    rowCount(parentIndex) + listTasks.size());

    for (auto employee : listTasks)
        employee->setParent(m_rootObject);

    endInsertRows();
}

// =============================================================================
