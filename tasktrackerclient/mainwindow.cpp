
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/connectdialog.h"
#include "dialogs/settingsdialog.h"

// =============================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbManager(new DbRemoteManager(this)),
    m_tasksViewer(new TasksViewer(m_dbManager, this)),
    m_employeesViewer(new EmployeesViewer(m_dbManager, this)),
    m_tasksModel(new TasksModel(m_dbManager, this)),
    m_employeesModel(new EmployeesModel(m_dbManager, this)),
    m_expiredTasksDelegate(new ExpiredTasksDelegate(m_tasksModel, this)),
    m_settings(new Settings("settings.ini", this))
{
    // Settings
    m_dbManager->setHostName(m_settings->hostName());
    m_dbManager->setPort(m_settings->port());

    // GUI
    ui->setupUi(this);

    // File
    connect(ui->actionNewDb, &QAction::triggered,
            this, &MainWindow::newDb);

    connect(ui->actionConnectToDb, &QAction::triggered,
            this, &MainWindow::openDb);

    connect(ui->actionCloseDb, &QAction::triggered,
            this, &MainWindow::closeDb);

    // Edit
    connect(ui->actionAddTask, &QAction::triggered,
            m_tasksViewer, &TasksViewer::create);

    connect(ui->actionAddSubTask, &QAction::triggered,
            m_tasksViewer, &TasksViewer::createSub);

    connect(ui->actionAddEmployee, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::create);

    connect(ui->actionEditTask, &QAction::triggered,
            m_tasksViewer, &TasksViewer::edit);

    connect(ui->actionEditEmployee, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::edit);

    connect(ui->actionDeleteTask, &QAction::triggered,
        m_tasksViewer, &TasksViewer::remove);

    connect(ui->actionDeleteEmployee, &QAction::triggered,
            m_employeesViewer, &EmployeesViewer::remove);

    connect(ui->actionUpdate, &QAction::triggered,
            m_tasksViewer, &TasksViewer::update);

    // Tools
    connect(ui->actionSettings, &QAction::triggered,
            this, &MainWindow::settings);

    // Tables
    connect(m_tasksViewer, &TasksViewer::gotoEmployee,
            this, [this](Employee* employee) {
                ui->tabWidget->setCurrentIndex(1);
                auto index = m_employeesModel->indexOf(employee);
                m_employeesViewer->selectItem(index);
            }
    );

    ui->tabWidget->addTab(m_tasksViewer, tr("Tasks"));
    m_tasksViewer->setModel(m_tasksModel);

    connect(m_expiredTasksDelegate, &ExpiredTasksDelegate::gotoItem,
            this, [this](const QModelIndex& index) {
                ui->tabWidget->setCurrentIndex(0);
                m_tasksViewer->selectItem(index);
            }
    );

    ui->tabWidget->addTab(m_employeesViewer, tr("Employees"));
    m_employeesViewer->setModel(m_employeesModel);
    m_employeesViewer->setExpiredTasksDelegate(m_expiredTasksDelegate);

    setViewDisconnected();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =============================================================================
void MainWindow::newDb() {
    connectServer(true);
}

void MainWindow::openDb() {
    connectServer(false);
}

// =============================================================================
void MainWindow::settings()
{
    auto dialog = QScopedPointer(new SettingsDialog(
        m_settings,
        this
    ));

    dialog->exec();
}

// =============================================================================
void MainWindow::closeDb()
{
    m_dbManager->setDbName("");
    setViewDisconnected();
}

// =============================================================================
void MainWindow::connectServer(bool isNeedCreate)
{
    auto dialog = QScopedPointer(new ConnectDialog(
        m_dbManager,
        this,
        isNeedCreate
    ));

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_tasksViewer->update();

    setViewConnected();
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

    // Edit
    ui->menuEdit->menuAction()->setVisible(false);
    ui->actionAddSubTask->setVisible(false);
    ui->actionEditTask->setVisible(false);
    ui->actionEditEmployee->setVisible(false);
    ui->actionDeleteTask->setVisible(false);
    ui->actionDeleteEmployee->setVisible(false);
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

    // Edit
    ui->menuEdit->menuAction()->setVisible(true);
}

// =============================================================================
