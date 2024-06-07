#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "httpclient/httpclient.h"

#include "models/employeesmodel.h"
#include "models/tasksmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newDb();
    void openDb();

    void newEmployee();
    void deleteEmployee();
    void updateEmployees();

    void newTask();
    void deleteTask();
    void updateTasks();

    void employeeViewDoubleClicked(const QModelIndex& index);
    void employeesViewContextMenu(const QPoint& pos);

    void tasksViewDoubleClicked(const QModelIndex& index);
    void tasksViewContextMenu(const QPoint& pos);

private:
    void setWindowDbName(const QString& name);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<HttpClient> m_httpClient;
    EmployeesModel* m_employeesModel;
    TasksModel* m_tasksModel;
};

#endif // MAINWINDOW_H
