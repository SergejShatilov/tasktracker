
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>

#include "dialogs/dialogopendb/dialogopendb.h"

// =============================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_httpClient(std::make_shared<HttpClient>(this)),
    m_tasksView(new TasksView(m_httpClient, this)),
    m_employeesView(new EmployeesView(m_httpClient, this))
{
    ui->setupUi(this);

    connect(ui->actionNewDb, &QAction::triggered,
            this, &MainWindow::newDb);

    connect(ui->actionConnectToDb, &QAction::triggered,
            this, &MainWindow::openDb);

    ui->tabTasks->setLayout(ui->tasksLayout);
    ui->tabEmployees->setLayout(ui->employeesLayout);
    ui->tasksLayout->addWidget(m_tasksView);
    ui->employeesLayout->addWidget(m_employeesView);

    uiViewDisconnected();
}

// =============================================================================
MainWindow::~MainWindow()
{
    delete ui;
}

// =============================================================================
void MainWindow::newDb()
{
    auto dialog = std::make_unique<DialogOpenDb>(m_httpClient, true, this);
    dialog->setWindowTitle(tr("Create new db..."));
    dialog->show();

    if (dialog->exec() != QDialog::Accepted)
        return;

    uiViewConnected();
    m_httpClient->setDbName(dialog->dbName());
    setWindowDbName(dialog->dbName());
}

// =============================================================================
void MainWindow::openDb()
{
    auto dialog = std::make_unique<DialogOpenDb>(m_httpClient, false, this);
    dialog->setWindowTitle(tr("Connect to db..."));
    dialog->show();

    if (dialog->exec() != QDialog::Accepted)
        return;

    uiViewConnected();
    m_httpClient->setDbName(dialog->dbName());
    setWindowDbName(dialog->dbName());

    m_tasksView->slotUpdate();
    m_employeesView->slotUpdate();

    //updateEmployees();
    //updateTasks();
}

// =============================================================================
void MainWindow::setWindowDbName(const QString &name)
{
    setWindowTitle(QString("%1 @ %2").arg(qApp->applicationName()).arg(name));
}

// =============================================================================
void MainWindow::uiViewDisconnected()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setVisible(false);

    setWindowTitle(qApp->applicationName());
}

// =============================================================================
void MainWindow::uiViewConnected()
{
    ui->tabWidget->setVisible(true);
}

// =============================================================================
