
#include "tasksmodel.h"

#include <QDebug>

// =============================================================================
TasksModel::TasksModel(QObject* parent) :
    QAbstractItemModel(parent),
    m_columns({"name", "id", "state", "executorId",
               "deadline", "parentId", "description"}),
    m_rootItem(new TaskObject(this)),
    m_employeesModel(nullptr)
{
}

// =============================================================================
TasksModel::~TasksModel()
{

}

// =============================================================================
void TasksModel::setEmployeesModel(EmployeesModel* model) {
    m_employeesModel = model;
}

// =============================================================================
void TasksModel::addTask(const Task& task, const QModelIndex& parentIndex)
{
    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));
qDebug() << task;
    TaskObject* taskObject = new TaskObject(task);
    m_listObjects << taskObject;

    auto parent = taskObjectByIndex(parentIndex);
    taskObject->setParent(parent);

    endInsertRows();
}

// =============================================================================
QList<qint32> TasksModel::removeTask(const QModelIndex& index)
{
    QList<qint32> ids;
    beginRemoveRows(QModelIndex(), index.row(), index.row() + 1);

    // Рекурсивное удаление всех дочерних задач
    std::function<void(const QObject* obj)> funcChildrenDelete;
    funcChildrenDelete = [this, &ids, &funcChildrenDelete](const QObject* obj)
    {
        const auto& children = obj->children();
        for (const auto& child : children)
        {
            // Если существуют дочерние элементы, то удаляем сначала их
            if (!child->children().isEmpty())
                funcChildrenDelete(child);

            auto taskObject = dynamic_cast<TaskObject*>(child);

            // Добавляем id в список id на удаление
            ids << taskObject->id();

            // Удаляем из списка
            m_listObjects.removeOne(taskObject);
        }
    };

    auto obj = taskObjectByIndex(index);

    // Удаляем все дочерние объекты из списка
    funcChildrenDelete(obj);
    ids << obj->id();

    // Удаляем сам объект, его дочерние объекты сами удалятся
    m_listObjects.removeOne(obj);
    delete obj;

    endRemoveRows();

    return ids;
}

// =============================================================================
void TasksModel::loadFromList(const QList<Task>& list)
{
    clear();

    for (const auto& task : list)
    {
        // Ищем объект-родитель
        auto parentObj = findById(task.parentId());

        // Ищем индекс родителя
        auto indexParent = indexOf(parentObj);

        addTask(task, indexParent);
    }
}

// =============================================================================
QObject* TasksModel::findById(qint32 id) const
{
    auto it = std::find_if
    (
        m_listObjects.cbegin(),
        m_listObjects.cend(),
        [id](const TaskObject* obj) {
            return (obj->id() == id);
        }
    );

    if (it == m_listObjects.cend())
        return nullptr;

    return *it;
}

// =============================================================================
QModelIndex TasksModel::indexOf(QObject* obj) const
{
    if (obj == nullptr)
        return QModelIndex();

    QObject* parentObj = obj->parent();

    int row = parentObj->children().indexOf(obj);

    return createIndex(row, 0, obj);
}

// =============================================================================
qint32 TasksModel::idByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    TaskObject* taskObject = taskObjectByIndex(index);
    return taskObject->id();
}

// =============================================================================
QString TasksModel::nameByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return QString("");

    TaskObject* taskObject = taskObjectByIndex(index);
    return taskObject->name();
}

// =============================================================================
TaskObject*
TasksModel::taskObjectByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return m_rootItem;

    return static_cast<TaskObject*>(index.internalPointer());
}

// =============================================================================
QModelIndex TasksModel::index(int row, int column,
                              const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QObject* parentObj = taskObjectByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

// =============================================================================
QModelIndex TasksModel::parent(const QModelIndex &child) const
{
    QObject* childObj = taskObjectByIndex(child);
    QObject* parentObj = childObj->parent();

    if (parentObj == m_rootItem)
        return QModelIndex();

    QObject* grandParentObj = parentObj->parent();

    int row = grandParentObj->children().indexOf(parentObj);

    return createIndex(row, 0, parentObj);
}

// =============================================================================
int TasksModel::rowCount(const QModelIndex &parent) const {
    return taskObjectByIndex(parent)->children().count();
}

// =============================================================================
int TasksModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_columns.size();
}

// =============================================================================
QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
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

            return obj->property(propertyName.toUtf8());
        }
        default:
            return QVariant();
    }
}

// =============================================================================
bool TasksModel::setData(const QModelIndex &index,
                             const QVariant &value,
                             int role)
{
    if (!index.isValid())
        return false;

    switch (role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            return taskObjectByIndex(index)->setProperty
            (
                m_columns.at(index.column()).toUtf8(),
                value
            );
        }
        default:
            return false;
    }
}

// =============================================================================
QVariant TasksModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if ((orientation != Qt::Orientation::Horizontal) ||
        (role != Qt::DisplayRole))
    {
        return QVariant();
    }

    switch (section)
    {
        case 0: return QString(tr("Name"));
        case 1: return QString(tr("ID"));
        case 2: return QString(tr("State"));
        case 3: return QString(tr("Executor"));
        case 4: return QString(tr("Deadline"));
        case 5: return QString(tr("Parent"));
        case 6: return QString(tr("Description"));
        default: return QVariant();
    }
}

// =============================================================================
void TasksModel::clear()
{
    beginResetModel();

    const auto& childred = m_rootItem->children();
    for (const auto child : childred) {
        delete child;
    }

    m_listObjects.clear();

    endResetModel();
}

// =============================================================================
