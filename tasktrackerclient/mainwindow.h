#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "httpclient/httpclient.h"
#include "views/tasksview.h"
#include "views/employeesview.h"

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

private:
    void setWindowDbName(const QString& name);
    void uiViewDisconnected();
    void uiViewConnected();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<HttpClient> m_httpClient;
    TasksView*     m_tasksView;
    EmployeesView* m_employeesView;
};

#endif // MAINWINDOW_H
