
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/connectdialog.h"

// =============================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbManager(new DbRemoteManager(this)),
    m_tasksViewer(new TasksViewer(m_dbManager, this)),
    m_employeesViewer(new EmployeesViewer(m_dbManager, this)),
    m_tasksModel(new TasksModel(m_dbManager, this)),
    m_employeesModel(new EmployeesModel(m_dbManager, this))
{
    ui->setupUi(this);

    // File
    connect(ui->actionNewDb, &QAction::triggered,
            this, &MainWindow::newDb);

    connect(ui->actionConnectToDb, &QAction::triggered,
            this, &MainWindow::openDb);

    connect(ui->actionCloseDb, &QAction::triggered,
            this, &MainWindow::closeDb);

    // Tasks
    connect(ui->actionTaskAdd, &QAction::triggered,
            m_tasksViewer, &TasksViewer::create);

    connect(ui->actionTaskAddSub, &QAction::triggered,
            m_tasksViewer, &TasksViewer::createSub);

    connect(ui->actionTaskEdit, &QAction::triggered,
            m_tasksViewer, &TasksViewer::edit);

    connect(ui->actionTaskDelete, &QAction::triggered,
            m_tasksViewer, &TasksViewer::remove);

    connect(ui->actionTasksUpdate, &QAction::triggered,
            m_tasksViewer, &TasksViewer::update);

    ui->tabWidget->addTab(m_tasksViewer, tr("Tasks"));
    m_tasksViewer->setModel(m_tasksModel);

    // Employees
    connect(ui->actionEmployeeAdd, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::create);

    connect(ui->actionEmployeeEdit, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::edit);

    connect(ui->actionEmployeeDelete, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::remove);

    connect(ui->actionEmployeesUpdate, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::update);

    ui->tabWidget->addTab(m_employeesViewer, tr("Employees"));
    m_employeesViewer->setModel(m_employeesModel);

    setViewDisconnected();
}

// =============================================================================
MainWindow::~MainWindow()
{
    delete ui;
}

// =============================================================================
void MainWindow::newDb()
{
    auto dialog = QScopedPointer(new ConnectDialog(
        m_dbManager,
        this,
        true
    ));

    if (dialog->exec() != QDialog::Accepted)
        return;

    setViewConnected();
}

// =============================================================================
void MainWindow::openDb()
{
    auto dialog = QScopedPointer(new ConnectDialog(
        m_dbManager,
        this
    ));

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_tasksViewer->update();

    setViewConnected();
}

// =============================================================================
void MainWindow::closeDb()
{
    m_dbManager->setDbName("");
    setViewDisconnected();
}

// =============================================================================
void MainWindow::setViewDisconnected()
{
    setWindowTitle(qApp->applicationName());

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setVisible(false);

    m_tasksViewer->reset();
    m_employeesViewer->reset();

    // File
    ui->actionCloseDb->setVisible(false);

    // Tasks
    ui->menuTasks->menuAction()->setVisible(false);
    ui->actionTaskAddSub->setVisible(false);
    ui->actionTaskEdit->setVisible(false);
    ui->actionTaskDelete->setVisible(false);

    // Employees
    ui->menuEmployees->menuAction()->setVisible(false);
    ui->actionEmployeeEdit->setVisible(false);
    ui->actionEmployeeDelete->setVisible(false);

}

// =============================================================================
void MainWindow::setViewConnected()
{
    setWindowTitle(QString("%1 @ %2")
                   .arg(qApp->applicationName())
                   .arg(m_dbManager->dbName())
    );

    ui->tabWidget->setVisible(true);

    // File
    ui->actionCloseDb->setVisible(true);

    // Tasks
    ui->menuTasks->menuAction()->setVisible(true);

    // Employees
    ui->menuEmployees->menuAction()->setVisible(true);
}

// =============================================================================
