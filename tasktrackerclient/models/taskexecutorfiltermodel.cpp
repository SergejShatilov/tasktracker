
#include "taskexecutorfiltermodel.h"
#include <QDebug>
#include <QDate>

// =============================================================================
TaskExecutorFilterModel::TaskExecutorFilterModel(QObject* parent) :
    QSortFilterProxyModel(parent),
    m_executorId(0)
{

}

// =============================================================================
void TaskExecutorFilterModel::setExecutorId(qint32 executorId) {
    m_executorId = executorId;
    invalidateFilter();
}

// =============================================================================
bool TaskExecutorFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //qDebug() << source_row << source_parent;

    QModelIndex index = sourceModel()->index(source_row, 3, source_parent);
    QVariant value = sourceModel()->data(index, Qt::ToolTipRole);

    if (value.isValid())
    {
        qint32 executorId = value.toInt();

        if (m_executorId == executorId)
        {
            index = sourceModel()->index(source_row, 2, source_parent);
            value = sourceModel()->data(index, Qt::ToolTipRole);

            if (value.toString() != "Completed")
            {
                index = sourceModel()->index(source_row, 4, source_parent);
                value = sourceModel()->data(index, Qt::ToolTipRole);

                qDebug() << value;

                if (value.toDate() < QDate::currentDate())
                    return true;
            }
        }

        return false;
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

// =============================================================================
