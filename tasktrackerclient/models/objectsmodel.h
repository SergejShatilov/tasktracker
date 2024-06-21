
#pragma once

#include <QAbstractItemModel>

#include "db/dbremotemanager.h"

class ObjectsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ObjectsModel(DbRemoteManager* dbManager,
                          QObject* parent = nullptr);

public:
    virtual QModelIndex index(int row,
                              int column,
                              const QModelIndex &parent) const override;

    virtual QModelIndex parent(const QModelIndex &child) const override;

    virtual int rowCount(const QModelIndex &parent) const override;

    virtual int columnCount(const QModelIndex &parent) const override;

    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role) const override;

    QVariant data(const QModelIndex &index, int role) const override;

public:
    int columnByField(const QString& field) const;
    const QString& fieldByColumn(int column) const;

    QObject* objectByIndex(const QModelIndex& index) const;
    QModelIndex indexOf(QObject* obj) const;

protected:
    bool isChild(QObject* obj) const;

    void addField(const QString& field,
                  const QString& header = QString(),
                  bool isVisible = true);
    void clearFields();

    void reset();

protected:
    virtual QVariant dataDisplayRole(const QModelIndex &index,
                                     QObject* obj,
                                     const QString& field) const = 0;
    virtual QVariant dataEditRole(const QModelIndex &index,
                                  QObject* obj,
                                  const QString& field) const = 0;
    virtual QVariant dataBackgroundRole(const QModelIndex &index,
                                        QObject* obj,
                                        const QString& field) const = 0;
    virtual QVariant dataForegroundRole(const QModelIndex &index,
                                        QObject* obj,
                                        const QString& field) const = 0;

protected:
    DbRemoteManager* m_dbManager;
    QObject*         m_rootObject;

private:
    QList<std::tuple<QString, QString, bool>> m_fields;

};
