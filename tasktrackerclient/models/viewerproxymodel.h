
#pragma once

#include <QSortFilterProxyModel>

class ViewerProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ViewerProxyModel(QObject* parent = nullptr);

public slots:
    void setFieldIndex(int fieldIndex);
    void setFilterValue(const QString& value);

protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;

private:
    int     m_fieldIndex;
    QString m_filterValue;

};
