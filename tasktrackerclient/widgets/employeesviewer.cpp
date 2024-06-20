
#include "employeesviewer.h"

#include <QMessageBox>

#include "dialogs/employeedialog.h"
#include "models/employeesmodel.h"

// =============================================================================
EmployeesViewer::EmployeesViewer(DbRemoteManager* dbManager, QWidget *parent) :
    Viewer(dbManager, parent)
{
}

// =============================================================================
void EmployeesViewer::setExpiredTasksDelegate(QAbstractItemDelegate* delegate)
{
    auto proxyModel = static_cast<QAbstractProxyModel*>(treeView()->model());
    auto model = static_cast<EmployeesModel*>(proxyModel->sourceModel());
    treeView()->setItemDelegateForColumn(
        model->columnByField("expiredTasks"), delegate
    );
}

// =============================================================================
void EmployeesViewer::createByIndex(const QModelIndex& index)
{
    Q_UNUSED(index);

    auto dialog = QScopedPointer(new EmployeeDialog(
        m_dbManager,
        this
    ));

    if (dialog->exec() != QDialog::Accepted)
        return;
}

// =============================================================================
void EmployeesViewer::editByIndex(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    auto employee = static_cast<Employee*>(index.internalPointer());
    if (employee == nullptr)
        return;

    auto dialog = QScopedPointer(new EmployeeDialog(
        m_dbManager,
        this,
        employee
    ));

    if (dialog->exec() != QDialog::Accepted)
        return;
}

// =============================================================================
void EmployeesViewer::removeByIndex(const QModelIndex& index)
{
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

    auto employee = static_cast<Employee*>(index.internalPointer());

    m_dbManager->removeEmployee(employee);
}

// =============================================================================
void EmployeesViewer::customContextMenu(QMenu* menu, const QModelIndex& index)
{
    menu->addAction(tr("Add..."), this, &Viewer::create);
    menu->addSeparator();

    // Если индекс валидный, предлагаем редактировать и удалить
    if (index.isValid())
    {
        menu->addAction(tr("Edit..."), this, [this, &index](){
            editByIndex(index);
        });

        menu->addAction(tr("Delete"), this, &Viewer::remove);
        menu->addSeparator();
    }

    menu->addAction(tr("Update"), this, &Viewer::update);
}

// =============================================================================
void EmployeesViewer::itemDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    auto field = index.model()->headerData(
        index.column(), Qt::Horizontal, (Qt::UserRole + 1)
    );

    // Колонку с задачами с истекщим сроком не обрабатываем двойным щелчком
    if (field == "expiredTasks")
        return;

    editByIndex(index);
}

// =============================================================================
