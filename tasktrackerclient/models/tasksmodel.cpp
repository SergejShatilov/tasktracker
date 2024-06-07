
#include "tasksmodel.h"

#include <QDebug>

// =============================================================================
TasksModel::TasksModel(QObject* parent) :
    QAbstractItemModel(parent),
    m_columns({"id", "name", "state", "executorId",
               "start", "duration", "parentId", "description"}),
    m_rootItem(new TaskObject(this))
{
}

// =============================================================================
TasksModel::~TasksModel()
{

}

// =============================================================================
void TasksModel::addTask(const Task& task)
{
    auto parentIndex = QModelIndex();

    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));

    TaskObject* taskObject = new TaskObject(task);
    taskObject->setParent(m_rootItem);

    endInsertRows();
}

// =============================================================================
void TasksModel::removeTask(const QModelIndex& index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row() + 1);
    auto obj = taskObjectByIndex(index);
    obj->deleteLater();
    endRemoveRows();
}

// =============================================================================
void TasksModel::loadFromList(const QList<Task>& list)
{
    clear();

    for (const auto& task : list) {
        addTask(task);
    }
}

// =============================================================================
qint32 TasksModel::idByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return -1;

    TaskObject* taskObject = taskObjectByIndex(index);
    return taskObject->id();
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
            return taskObjectByIndex(index)->property
            (
                m_columns.at(index.column()).toUtf8()
            );
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
        case 0: return QString(tr("ID"));
        case 1: return QString(tr("Name"));
        case 2: return QString(tr("State"));
        case 3: return QString(tr("Executor"));
        case 4: return QString(tr("Start"));
        case 5: return QString(tr("Duration"));
        case 6: return QString(tr("Parent"));
        case 7: return QString(tr("Description"));
        default: return QVariant();
    }
}

// =============================================================================
void TasksModel::clear()
{
    beginResetModel();

    const auto& objectsList = m_rootItem->children();
    for (const auto& obj : objectsList) {
        obj->deleteLater();
    }

    endResetModel();
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
