
#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

#include "db/dbremotemanager.h"

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(DbRemoteManager* dbManager,
                           QWidget *parent = nullptr,
                           bool isNewDb = false);
    ~ConnectDialog();

    QString dbName() const;

private slots:
    void clickedOk();
    void changedName(const QString& name);

private:
    Ui::ConnectDialog* ui;
    DbRemoteManager*   m_dbManager;

    std::function<bool(const QString& str)> m_funcHandler;
};

#endif // CONNECTDIALOG_H
