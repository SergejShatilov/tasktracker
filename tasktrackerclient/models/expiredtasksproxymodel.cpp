
#include "expiredtasksproxymodel.h"

#include <QDate>

#include "tasksmodel.h"

// =============================================================================
ExpiredTasksProxyModel::ExpiredTasksProxyModel(QObject* parent) :
    QSortFilterProxyModel(parent),
    m_executorId(0)
{

}

// =============================================================================
void ExpiredTasksProxyModel::setExecutorId(qint32 executorId) {
    m_executorId = executorId;
    invalidateFilter();
}

// =============================================================================
bool ExpiredTasksProxyModel::filterAcceptsRow(
    int source_row,
    const QModelIndex &source_parent) const
{
    if (m_executorId == 0)
        return false;

    auto tasksModel = static_cast<TasksModel*>(sourceModel());
    int column = tasksModel->columnByField("executorId");

    QModelIndex index = sourceModel()->index(
        source_row, column, source_parent
    );

    if (!index.isValid())
        return false;

    QVariant value = sourceModel()->data(index);
    if (value.isValid())
    {
        qint32 executorId = value.toInt();
        if (m_executorId == executorId)
        {
            // Получаем состояние задачи
            auto stateIndex = sourceModel()->index(
                source_row,
                tasksModel->columnByField("stateString"),
                source_parent
            );

            auto stateString = sourceModel()->data(stateIndex).toString();

            // Если задача не выполнена
            if (stateString != "Completed")
            {
                // Запрашиваем срок задачи
                auto stateDeadline = sourceModel()->index(
                    source_row,
                    tasksModel->columnByField("deadline"),
                    source_parent
                );

                auto deadline = sourceModel()->data(stateDeadline).toDate();

                // Если дата после сегодняшнего числа
                // (не включая седняшнее число), то она считается просроченной
                if (deadline < QDate::currentDate())
                    return true;
            }
        }
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
