
#include "employeesmodel.h"

#include "employeeobject.h"
#include <QDebug>

// =============================================================================
EmployeesModel::EmployeesModel(QObject* parent) :
    QAbstractItemModel(parent),
    m_columns({"id", "lastName", "firstName", "patronymic",
               "fullName", "position", "email", "phone"}),
    m_rootItem(new EmployeeObject(this))
{
}

// =============================================================================
EmployeesModel::~EmployeesModel()
{

}

// =============================================================================
void EmployeesModel::addEmployee(const Employee& employee)
{
    auto parentIndex = QModelIndex();

    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));

    EmployeeObject* employeeObject = new EmployeeObject(employee);
    employeeObject->setParent(m_rootItem);

    endInsertRows();
}

// =============================================================================
void EmployeesModel::removeEmployee(const QModelIndex& index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row() + 1);
    auto obj = employeeObjectByIndex(index);
    obj->deleteLater();
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
            return employeeObjectByIndex(index)->property
            (
                m_columns.at(index.column()).toUtf8()
            );
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
        default: return QVariant();
    }
}

// =============================================================================
void EmployeesModel::clear()
{
    beginResetModel();

    const auto& objectsList = m_rootItem->children();
    for (const auto& obj : objectsList) {
        obj->deleteLater();
    }

    endResetModel();
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
