
#include "tasksmodel.h"

#include <QBrush>
#include <QDebug>

// =============================================================================
TasksModel::TasksModel(DbRemoteManager* dbManager, QObject* parent) :
    ObjectsModel(dbManager, parent)
{
    addField("name",            tr("Name"),        true);
    addField("id",              tr("ID"),          true);
    addField("state",           tr("State_"),      true);
    addField("stateDisplay",    tr("State"),       true);
    addField("executorId",      tr("Executor ID"), false);
    addField("executor",        tr("Executor"),    true);
    addField("deadline",        tr("Deadline"),    true);
    addField("parentId",        tr("Parent ID"),   true);
    addField("description",     tr("Description"), true);

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
                return -1;
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
        default:
            return QVariant();
    }

    /*switch (role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            const auto& propertyName = m_columns.at(index.column());
            const auto obj = taskObjectByIndex(index);

            // Если колонка с исполнителем
            if (propertyName == "executorId" && m_employeesModel != nullptr)
            {
                return m_employeesModel->fullNameById(obj->executorId());
            }

            // Если колонка с состоянием
            if (propertyName == "state")
            {
                const QString state = obj->stateString();

                static QHash<QString, QString> const tableStates = {
                    {"NotStarted",  tr("Not Started")},
                    {"Work",        tr("Work")},
                    {"Suspended",   tr("Suspended")},
                    {"Completed",   tr("Completed")}
                };

                auto it = tableStates.find(state);
                if (it == tableStates.end())
                    return obj->property(propertyName.toUtf8());

                return *it;
            }

            return obj->property(propertyName.toUtf8());
        }
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
    /*if (!index.isValid())
        return false;

    switch (role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            // Если изменение состояния
            if (index.column() == 2)
            {
                auto taskObject = taskObjectByIndex(index);
                taskObject->setStateString(value.toString());

                Task task;
                task.setId(taskObject->id());
                task.setName(taskObject->name());
                task.setState(taskObject->state());
                task.setDeadline(taskObject->deadline());
                task.setParentId(taskObject->parentId());
                task.setExecutorId(taskObject->executorId());
                task.setDescription(taskObject->description());

                m_httpClient->changeTask(task.id(), task);

                return true;
            }

            return taskObjectByIndex(index)->setProperty
            (
                m_columns.at(index.column()).toUtf8(),
                value
            );
        }
        default:
            return false;
    }*/

    return false;
}

// =============================================================================
Qt::ItemFlags TasksModel::flags(const QModelIndex& index) const
{
    /*if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    // Для колонки состояния, делает так чтобы ее можно было редактировать
    if (index.column() == 2)
        flags |= Qt::ItemIsEditable;

    return flags;*/

    return ObjectsModel::flags(index);
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
