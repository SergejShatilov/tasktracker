
#include "employeesmodel.h"

#include "employeeobject.h"
#include <QDebug>
#include "tasksmodel.h"
#include <QBrush>

// =============================================================================
EmployeesModel::EmployeesModel(QObject* parent) :
    QAbstractItemModel(parent),
    m_columns({"id", "lastName", "firstName", "patronymic",
               "fullName", "position", "email", "phone", "tasks"}),
    m_rootItem(new EmployeeObject(this)),
    m_tasksModel(nullptr)
{
}

// =============================================================================
EmployeesModel::~EmployeesModel()
{

}

// =============================================================================
void EmployeesModel::setTasksModel(QAbstractItemModel* model) {
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

// =============================================================================
QModelIndex EmployeesModel::index(int row, int column,
                                  const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QObject* parentObj = employeeObjectByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

// =============================================================================
QModelIndex EmployeesModel::parent(const QModelIndex &child) const
{
    QObject* childObj = employeeObjectByIndex(child);
    QObject* parentObj = childObj->parent();

    if (parentObj == m_rootItem)
        return QModelIndex();

    QObject* grandParentObj = parentObj->parent();

    int row = grandParentObj->children().indexOf(parentObj);

    return createIndex(row, 0, parentObj);
}

// =============================================================================
int EmployeesModel::rowCount(const QModelIndex &parent) const {
    return employeeObjectByIndex(parent)->children().count();
}

// =============================================================================
int EmployeesModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_columns.size();
}

// =============================================================================
QVariant EmployeesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

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
    }
}

// =============================================================================
bool EmployeesModel::setData(const QModelIndex &index,
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
            return employeeObjectByIndex(index)->setProperty
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
QVariant EmployeesModel::headerData(int section,
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
        case 4: return QString(tr("Full Name"));
        case 5: return QString(tr("Position"));
        case 6: return QString(tr("Email"));
        case 7: return QString(tr("Phone"));
        case 8: return QString(tr("Deadline Tasks"));
        default: return QVariant();
    }
}

// =============================================================================
Qt::ItemFlags EmployeesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == 8)
        flags |= Qt::ItemIsEditable;

    return flags;
}

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
}

// =============================================================================
EmployeeObject*
EmployeesModel::employeeObjectByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return m_rootItem;

    return static_cast<EmployeeObject*>(index.internalPointer());
}

// =============================================================================
void EmployeesModel::clear()
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
