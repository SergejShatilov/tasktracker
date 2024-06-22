#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "db/dbremotemanager.h"
#include "widgets/tasksviewer.h"
#include "widgets/employeesviewer.h"
#include "models/tasksmodel.h"
#include "models/employeesmodel.h"
#include "delegates/expiredtasksdelegate.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Settings* settings, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newDb();
    void openDb();
    void closeDb();
    void settings();

private:
    void connectServer(bool isNeedCreate);
    void setViewDisconnected();
    void setViewConnected();

private:
    Ui::MainWindow*         ui;
    DbRemoteManager*        m_dbManager;
    TasksViewer*            m_tasksViewer;
    EmployeesViewer*        m_employeesViewer;
    TasksModel*             m_tasksModel;
    EmployeesModel*         m_employeesModel;
    ExpiredTasksDelegate*   m_expiredTasksDelegate;
    Settings*               m_settings;
};

#endif // MAINWINDOW_H
