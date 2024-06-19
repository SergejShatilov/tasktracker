
#include "viewerproxymodel.h"

// =============================================================================
ViewerProxyModel::ViewerProxyModel(QObject* parent) :
    QSortFilterProxyModel(parent),
    m_fieldIndex(0),
    m_filterValue(QString(""))
{
}

// =============================================================================
void ViewerProxyModel::setFieldIndex(int fieldIndex)
{
    m_fieldIndex = fieldIndex;
    invalidate();
}

void ViewerProxyModel::setFilterValue(const QString& value)
{
    m_filterValue = value;
    invalidate();
}

// =============================================================================
bool ViewerProxyModel::filterAcceptsRow(
    int source_row,
    const QModelIndex &source_parent) const
{
    if (m_filterValue.isEmpty())
    {
        return QSortFilterProxyModel::filterAcceptsRow(
            source_row, source_parent
        );
    }

    QModelIndex index = sourceModel()->index(
        source_row, m_fieldIndex, source_parent
    );

    if (!index.isValid())
        return false;

    QVariant value = sourceModel()->data(index, Qt::DisplayRole);
    if (value.isValid())
    {
        const QString str = value.toString();
        if (str.contains(m_filterValue, Qt::CaseInsensitive))
            return true;
    }

    // Пробегаем рекурсивно все дочерние, до первого условия,
    // удовлетворяющему фильтру
    for (int i = 0; i < sourceModel()->rowCount(index); ++i)
    {
        if (filterAcceptsRow(i, index))
            return true;
    }

    return false;
}

// =============================================================================
