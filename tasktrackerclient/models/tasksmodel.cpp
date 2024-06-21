
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

            // Если отображается срок
            if (field == "deadline")
            {
                // Если имеются подзадачи, то дата определяются
                // самой поздней подзадачей
                /*if (!task->children().isEmpty())
                {
                    std::function<QDate(QObject* parent)> funcFindDateLate;
                    funcFindDateLate = [&funcFindDateLate](QObject* parent)
                    {
                        auto task = static_cast<Task*>(parent);
                        auto date = task->deadline();

                        for (auto child : task->children())
                        {
                            auto childredDate = funcFindDateLate(child);
                            if (date < childredDate)
                                date = childredDate;
                        }

                        return date;
                    };

                    return funcFindDateLate(task);
                }*/

                return task->deadline();
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
        case Qt::BackgroundRole:
        {
            // Если отображаемое состояние задачи
            if (field == "stateDisplay")
            {
                switch(task->state())
                {
                    case Task::State::Suspended:
                        return QBrush(QColor(50, 50, 50));
                    case Task::State::Completed:
                        return QBrush(QColor(100, 150, 120));
                    default:
                        break;
                }
            }

            // Если срок задачи
            if (field == "deadline")
            {
                // Если задача завершена, то уже не важно
                if (task->state() == Task::State::Completed)
                    return QVariant();

                // Если задача просрочена
                if (task->deadline() < QDate::currentDate())
                    return QBrush(QColor(200, 120, 120));

                // Если дедлайн сегодня
                if (task->deadline() == QDate::currentDate())
                    return QBrush(QColor(240, 240, 160));
            }

            // Если задача корневая и она имеет подзадачи
            if (task->parentId() == 0 && !task->children().isEmpty())
                return QBrush(QColor(220, 220, 220));

            return QVariant();
        }
        case Qt::ForegroundRole:
        {
            // Если отображаемое состояние задачи
            if (field == "stateDisplay")
            {
                switch(task->state())
                {
                    case Task::State::NotStarted:
                        return QColor(Qt::darkGray);
                    case Task::State::Suspended:
                        return QColor(Qt::white);
                    case Task::State::Completed:
                        return QColor(Qt::white);
                    default:
                        return QVariant();
                }
            }

            // Если срок задачи
            if (field == "deadline")
            {
                // Если задача завершена, то уже не важно
                if (task->state() == Task::State::Completed)
                    return QVariant();

                // Если задача просрочена
                if (task->deadline() < QDate::currentDate())
                    return QColor(Qt::white);

                // Если дедлайн седня
                if (task->deadline() == QDate::currentDate())
                    return QColor(Qt::darkGray);
            }

            // Если отображается исполнитель
            if (field == "executor")
            {
                if (task->executor() == nullptr || task->executorId() == 0)
                    return QColor(Qt::darkGray);
            }

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
    {
        // Если у задачи есть подзадачи, то состояние ее менять нельзя
        auto task = static_cast<Task*>(index.internalPointer());
        if (task->children().isEmpty())
            flags |= Qt::ItemIsEditable;
    }

    return flags;
}

// =============================================================================
void TasksModel::addTask(Task* task)
{
    QObject* parent = task->parent();

    // Если родитель не найден во всей модели,
    // то родителем назначаем корневой объект
    if (!isChild(parent))
        parent = m_rootObject;

    const QModelIndex parentIndex = indexOf(parent);

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
