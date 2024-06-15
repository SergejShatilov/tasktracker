
#include "employeesview.h"

#include <QMenu>
#include <QMessageBox>
#include "dialogs/dialogeditemployee/dialogeditemployee.h"
#include "views/tasksdelegate.h"

// =============================================================================
EmployeesView::EmployeesView(HttpClient* httpClient,
                             QWidget* parent) :
    QTreeView(parent),
    m_httpClient(httpClient),
    m_employeesModel(new EmployeesModel(this)),
    m_tasksModel(nullptr)
{
    setModel(m_employeesModel);
    setColumnHidden(0, true);
    setColumnHidden(1, true);
    setColumnHidden(2, true);
    setColumnHidden(3, true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QTreeView::customContextMenuRequested,
            this, &EmployeesView::slotContextMenu);

    connect(this, &QTreeView::doubleClicked,
            this, &EmployeesView::slotDoubleClicked);
}

// =============================================================================
EmployeesModel* EmployeesView::employeesModel() const {
    return m_employeesModel;
}

// =============================================================================
void EmployeesView::setTasksModel(TasksModel* model) {
    m_tasksModel = model;
    m_employeesModel->setTasksModel(model);

    TasksDelegate* delegate = new TasksDelegate(model, this);
    setItemDelegateForColumn(8, delegate);
}

// =============================================================================
void EmployeesView::slotUpdate()
{
    QList<Employee> listEmployees;
    if (!m_httpClient->getEmployees(listEmployees))
        return;

    m_employeesModel->loadFromList(listEmployees);
}

// =============================================================================
void EmployeesView::slotCreate()
{
    auto dialog = std::make_unique<DialogEditEmployee>
    (
        m_httpClient,
        false,
        this
    );

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_employeesModel->addEmployee(dialog->employee());
}

// =============================================================================
void EmployeesView::slotDelete()
{
    auto index = currentIndex();

    if (!index.isValid())
        return;

    QMessageBox::StandardButton reply = QMessageBox::question
    (
        this,
        tr("Delete employee?"),
        tr("After deleting an employee, it will be impossible to restore it. "
           "His tasks will not be assigned.\r\n"
           "Are you sure you want to delete an employee?"),
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
void EmployeesView::slotEdit(const QModelIndex& index)
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
void EmployeesView::slotDoubleClicked(const QModelIndex& index)
{
    if (index.column() == 8)
        return;

    slotEdit(index);
}

// =============================================================================
void EmployeesView::slotContextMenu(const QPoint& pos)
{
    QModelIndex index = indexAt(pos);

    QMenu menu;
    menu.addAction(tr("Add..."), this, &EmployeesView::slotCreate);
    menu.addSeparator();

    if (index.isValid())
    {
        menu.addAction(tr("Edit..."), this, [this, &index](){
            slotEdit(index);
        });

        menu.addAction(tr("Delete"), this, &EmployeesView::slotDelete);
        menu.addSeparator();

    }

    menu.addAction(tr("Update"), this, &EmployeesView::slotUpdate);
    menu.exec(QCursor::pos());
}

// =============================================================================
