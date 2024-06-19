#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>

#include "db/dbremotemanager.h"

namespace Ui {
class TaskDialog;
}

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(DbRemoteManager* dbManager,
                        QWidget *parent = nullptr,
                        Task* task = nullptr);
    ~TaskDialog();

    const Task* task() const;

    void setTaskParent(Task* taskParent);

private slots:
    void clickedOk();
    void changed(const QString& str);

private:
    Ui::TaskDialog*  ui;
    DbRemoteManager* m_dbManager;
    Task*            m_task;

    std::function<bool(Task* task)> m_funcHandler;
};

#endif // TASKDIALOG_H
