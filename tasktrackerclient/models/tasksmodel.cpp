
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
QVariant TasksModel::dataDisplayRole(const QModelIndex &index,
                                     QObject* obj,
                                     const QString& field) const
{
    Q_UNUSED(index);

    auto task = static_cast<Task*>(obj);
    auto executor = static_cast<Employee*>(task->executor());

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
    else if (field == "executor")
    {
        // Если исполнитель не назначен, то отображаем "<Не выбрано>"
        if (executor == nullptr)
            return QString(tr("<Not Selected>"));

        return executor->fullName();
    }

    // Если отображается срок
    else if (field == "deadline")
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

// =============================================================================
QVariant TasksModel::dataEditRole(const QModelIndex &index,
                                  QObject* obj,
                                  const QString& field) const
{
    Q_UNUSED(index);
    auto task = static_cast<Task*>(obj);

    // Для отображаемого состояния возвращаем индекс этого состояния
    if (field == "stateDisplay")
        return task->stateIndex();

    return QVariant();
}

// =============================================================================
QVariant TasksModel::dataBackgroundRole(const QModelIndex &index,
                                        QObject* obj,
                                        const QString& field) const
{
    Q_UNUSED(index);
    auto task = static_cast<Task*>(obj);

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
    else if (field == "deadline")
    {
        // Если задача завершена, то уже не важно
        if (task->state() == Task::State::Completed)
            return QVariant();

        // Если задача просрочена
        else if (task->deadline() < QDate::currentDate())
            return QBrush(QColor(200, 120, 120));

        // Если дедлайн сегодня
        else if (task->deadline() == QDate::currentDate())
            return QBrush(QColor(240, 240, 160));
    }

    // Если задача корневая и она имеет подзадачи
    if (task->parentId() == 0 && !task->children().isEmpty())
        return QBrush(QColor(220, 220, 220));

    return QVariant();
}

// =============================================================================
QVariant TasksModel::dataForegroundRole(const QModelIndex &index,
                                        QObject* obj,
                                        const QString& field) const
{
    Q_UNUSED(index);
    auto task = static_cast<Task*>(obj);

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
    else if (field == "deadline")
    {
        // Если задача завершена, то уже не важно
        if (task->state() == Task::State::Completed)
            return QVariant();

        // Если задача просрочена
        else if (task->deadline() < QDate::currentDate())
            return QColor(Qt::white);

        // Если дедлайн седня
        else if (task->deadline() == QDate::currentDate())
            return QColor(Qt::darkGray);
    }

    // Если отображается исполнитель
    else if (field == "executor")
    {
        if (task->executor() == nullptr || task->executorId() == 0)
            return QColor(Qt::darkGray);
    }

    return QVariant();
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
