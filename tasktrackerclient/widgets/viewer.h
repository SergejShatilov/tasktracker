#ifndef VIEWER_H
#define VIEWER_H

#include <QMenu>
#include <QWidget>
#include <QTreeView>
#include "db/dbremotemanager.h"
#include "models/viewerproxymodel.h"

namespace Ui {
class Viewer;
}

class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(DbRemoteManager* dbManager,
                    QWidget *parent = nullptr);
    virtual ~Viewer();

    void setModel(QAbstractItemModel* model);

public slots:
    void create();
    void createSub();
    void remove();
    void edit();
    void update();
    void reset();

protected:
    virtual void createByIndex(const QModelIndex& index) = 0;
    virtual void editByIndex(const QModelIndex& index) = 0;
    virtual void removeByIndex(const QModelIndex& index) = 0;
    virtual void customContextMenu(QMenu* menu, const QModelIndex& index) = 0;
    virtual void itemDoubleClicked(const QModelIndex& index) = 0;

protected:
    QTreeView* treeView() const;

protected:
    DbRemoteManager* m_dbManager;

private slots:
    void showCustomContextMenu(const QPoint& pos);
    void doubleClicked(const QModelIndex& proxyIndex);
    void columnSelect(const QString& str);

private:
    QModelIndex currentIndex() const;

private:
    Ui::Viewer*       ui;
    ViewerProxyModel* m_proxyModel;
};

#endif // VIEWER_H
