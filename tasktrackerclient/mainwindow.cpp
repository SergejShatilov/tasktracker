
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

    connect(ui->treeView, &QTreeView::doubleClicked,
            this, &MainWindow::employeeViewDoubleClicked);

    setWindowTitle(qApp->applicationName());

    m_employeesModel = new EmployeesModel(this);
    ui->treeView->setModel(m_employeesModel);
    ui->treeView->setColumnHidden(0, true);
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            this, &MainWindow::employeesViewContextMenu);
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
    auto dialog = std::make_unique<DialogEditEmployee>(m_httpClient,
                                                       false, this);
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
        tr("After deleting an employee, it will be impossible to restore it. "
           "His tasks will not be assigned. Are you sure you want to delete an employee?"),
        QMessageBox::Yes | QMessageBox::Cancel
    );

    if (reply != QMessageBox::Yes)
        return;

    auto id = m_employeesModel->idByIndex(index);

    if (!m_httpClient->deleteEmployee(id))
        return;

    m_employeesModel->removeEmployee(index);
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
void MainWindow::employeeViewDoubleClicked(const QModelIndex& index)
{
    auto dialog = std::make_unique<DialogEditEmployee>
    (
        m_httpClient,
        true,
        this
    );

    dialog->setModel(m_employeesModel);
    dialog->setCurrentModelIndex(index);

    if (dialog->exec() != QDialog::Accepted)
        return;
}

// =============================================================================
void MainWindow::employeesViewContextMenu(const QPoint& pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);

    QMenu menu;
    menu.addAction(QStringLiteral("New..."), this, &MainWindow::newEmployee);
    menu.addSeparator();

    if (index.isValid())
    {
        menu.addAction(QStringLiteral("Edit..."), this, [this, &index](){
            employeeViewDoubleClicked(index);
        });
        menu.addAction(QStringLiteral("Delete"), this,
                       &MainWindow::deleteEmployee);
        menu.addSeparator();

    }

    menu.addAction(QStringLiteral("Update"), this, &MainWindow::updateEmployees);
    menu.exec(QCursor::pos());
}

// =============================================================================
void MainWindow::setWindowDbName(const QString &name)
{
    setWindowTitle(QString("%1 @ %2").arg(qApp->applicationName()).arg(name));
}

// =============================================================================
