
#include "objectsmodel.h"

// =============================================================================
ObjectsModel::ObjectsModel(DbRemoteManager* dbManager,
                           QObject* parent) :
    QAbstractItemModel(parent),
    m_dbManager(dbManager),
    m_rootObject(new QObject(this))
{
}

// =============================================================================
QModelIndex ObjectsModel::index(int row,
                                int column,
                                const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QObject* parentObj = objectByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

// =============================================================================
QModelIndex ObjectsModel::parent(const QModelIndex &child) const
{
    QObject* childObj = objectByIndex(child);
    QObject* parentObj = childObj->parent();

    if (parentObj == m_rootObject)
        return QModelIndex();

    QObject* grandParentObj = parentObj->parent();

    int row = grandParentObj->children().indexOf(parentObj);

    return createIndex(row, 0, parentObj);
}

// =============================================================================
int ObjectsModel::rowCount(const QModelIndex &parent) const {
    return objectByIndex(parent)->children().count();
}

int ObjectsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_fields.size();
}

// =============================================================================
QVariant ObjectsModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    if ((section < 0) ||
        (section >= m_fields.size()) ||
        (orientation != Qt::Horizontal))
    {
        return QVariant();
    }

    switch (role)
    {
        case Qt::DisplayRole:
            return std::get<1>(m_fields.at(section));
        case Qt::UserRole:
            return std::get<2>(m_fields.at(section));
        case Qt::UserRole + 1:
            return std::get<0>(m_fields.at(section));
        default:
            return QVariant();
    }
}

// =============================================================================
int ObjectsModel::columnByField(const QString& field) const
{
    auto it = std::find_if(
        m_fields.cbegin(),
        m_fields.cend(),
        [&field](const auto& field_) {
            return std::get<0>(field_) == field;
        }
    );

    return m_fields.indexOf(*it);
}

const QString& ObjectsModel::fieldByColumn(int column) const {
    return std::get<0>(m_fields.at(column));
}

// =============================================================================
QObject* ObjectsModel::objectByIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return m_rootObject;

    return static_cast<QObject*>(index.internalPointer());
}

// =============================================================================
QModelIndex ObjectsModel::indexOf(QObject* obj) const
{
    if (obj == m_rootObject)
        return QModelIndex();

    QObject* parent = obj->parent();

    int row = parent->children().indexOf(obj);

    return createIndex(row, 0, obj);
}

// =============================================================================
bool ObjectsModel::isChild(QObject* obj) const
{
    if (obj == nullptr)
        return false;

    if (obj == m_rootObject)
        return true;

    std::function<bool(QObject* parent)> funcIsChild;

    // Проверяет, является ли obj дочерним у parent
    funcIsChild = [obj, &funcIsChild](QObject* parent)
    {
        // Пробегаем по всем дочерним объектам
        for (auto child : parent->children())
        {
            // Если объект не является дочерним, и у него у самого
            // нет дочерних, то переходим к следующему
            if (child == obj) {
                return true;
            } else if (child->children().isEmpty()) {
                continue;
            }

            if (funcIsChild(child))
                return true;
        }

        return false;
    };

    return funcIsChild(m_rootObject);
}

// =============================================================================
void ObjectsModel::addField(const QString& field,
                            const QString& header,
                            bool isVisible)
{
    m_fields.append(std::make_tuple(field, header, isVisible));
}

void ObjectsModel::clearFields() {
    m_fields.clear();
}

// =============================================================================
void ObjectsModel::reset()
{
    beginResetModel();

    const auto& childred = m_rootObject->children();
    for (const auto child : childred) {
        delete child;
    }

    endResetModel();
}

// =============================================================================
