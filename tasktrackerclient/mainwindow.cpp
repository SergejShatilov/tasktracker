
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <memory>

#include "dialogs/dialogopendb/dialogopendb.h"
#include "dialogs/dialogeditemployee/dialogeditemployee.h"

// =============================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_httpClient(std::make_shared<HttpClient>(this))
{
    ui->setupUi(this);

    connect(ui->actionNewDb, &QAction::triggered,
            this, &MainWindow::newDb);

    connect(ui->actionConnectToDb, &QAction::triggered,
            this, &MainWindow::openDb);

    connect(ui->actionNewEmployee, &QAction::triggered,
            this, &MainWindow::newEmployee);

    connect(ui->actionDeleteEmployee, &QAction::triggered,
            this, &MainWindow::deleteEmployee);

    setWindowTitle(qApp->applicationName());

    m_employeesModel = new EmployeesModel(this);
    ui->treeView->setModel(m_employeesModel);

    //ui->treeView.add
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

    m_httpClient->setDbName(dialog->dbName());
    setWindowDbName(dialog->dbName());

    updateEmployees();
}

// =============================================================================
void MainWindow::newEmployee()
{
    auto dialog = std::make_unique<DialogEditEmployee>(m_httpClient, this);
    dialog->show();
    if (dialog->exec() != QDialog::Accepted)
        return;

    m_employeesModel->addEmployee(dialog->employee());
}

// =============================================================================
void MainWindow::deleteEmployee()
{
    auto index = ui->treeView->currentIndex();

    if (!index.isValid())
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Delete employee?"),
        tr("After deleting an employee, it will be impossible to restore it."
           "His tasks will not be assigned. Are you sure you want to delete an employee?"),
        QMessageBox::Yes | QMessageBox::Cancel
    );

    if (reply != QMessageBox::Yes)
        return;

    auto id = m_employeesModel->idByIndex(index);

    if (!m_httpClient->deleteEmployee(id))
        return;

    updateEmployees(); // FIXME: лучше заменить на удаление из модели
}

// =============================================================================
void MainWindow::updateEmployees()
{
    QList<Employee> listEmployees;
    if (!m_httpClient->getEmployees(listEmployees))
        return;

    m_employeesModel->loadFromList(listEmployees);
}

// =============================================================================
void MainWindow::setWindowDbName(const QString &name)
{
    setWindowTitle(QString("%1 @ %2").arg(qApp->applicationName()).arg(name));
}

// =============================================================================
