
#pragma once

#include <QSortFilterProxyModel>

class TaskExecutorFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TaskExecutorFilterModel(QObject* parent = nullptr);

    void setExecutorId(qint32 executorId);

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;

private:
    qint32 m_executorId;





};
