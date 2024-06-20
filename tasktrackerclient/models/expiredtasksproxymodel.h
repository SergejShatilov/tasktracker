
#pragma once

#include <QSortFilterProxyModel>

class ExpiredTasksProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ExpiredTasksProxyModel(QObject* parent = nullptr);

    void setExecutorId(qint32 executorId);

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;

private:
    qint32 m_executorId;

};
