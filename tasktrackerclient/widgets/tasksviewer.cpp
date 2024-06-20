
#include "tasksviewer.h"

#include <QMessageBox>

#include "dialogs/taskdialog.h"
#include "models/tasksmodel.h"

// =============================================================================
TasksViewer::TasksViewer(DbRemoteManager* dbManager, QWidget *parent) :
    Viewer(dbManager, parent),
    m_stateDelegate(new StateDelegate(this))
{
}

// =============================================================================
void TasksViewer::setModel(QAbstractItemModel* model)
{
    Viewer::setModel(model);

    auto tasksModel = static_cast<TasksModel*>(model);

    treeView()->setItemDelegateForColumn(
        tasksModel->columnByField("stateDisplay"),
        m_stateDelegate
    );
}

// =============================================================================
void TasksViewer::createByIndex(const QModelIndex& index)
{
    auto dialog = QScopedPointer(new TaskDialog(
        m_dbManager,
        this
    ));

    if (index.isValid())
        dialog->setTaskParent(static_cast<Task*>(index.internalPointer()));

    if (dialog->exec() != QDialog::Accepted)
        return;

    treeView()->expand(index);
}

// =============================================================================
void TasksViewer::editByIndex(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    auto task = static_cast<Task*>(index.internalPointer());
    if (task == nullptr)
        return;

    auto dialog = QScopedPointer(new TaskDialog(
        m_dbManager,
        this,
        task
    ));

    if (dialog->exec() != QDialog::Accepted)
        return;
}

// =============================================================================
void TasksViewer::removeByIndex(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    QMessageBox::StandardButton reply = QMessageBox::question
    (
        this,
        tr("Delete task?"),
        tr("Deleting a task will delete the entire subtask tree.\r\n"
           "Are you sure you want to delete the task?"),
        QMessageBox::Yes | QMessageBox::Cancel
    );

    if (reply != QMessageBox::Yes)
        return;

    auto task = static_cast<Task*>(index.internalPointer());

    treeView()->setCurrentIndex(QModelIndex());
    m_dbManager->removeTask(task, false);
}

// =============================================================================
void TasksViewer::customContextMenu(QMenu* menu, const QModelIndex& index)
{
    // Если индекс валидный, то предлагаем создать подзадачу
    if (index.isValid()) {
        menu->addAction(tr("Add Subtask..."), this, [this, &index](){
            createByIndex(index);
        });
    } else {
        menu->addAction(tr("Add..."), this, &Viewer::create);
    }

    menu->addSeparator();

    // Если индекс валидный, то предлагаем редактировать и удалять
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
void TasksViewer::itemDoubleClicked(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    auto field = index.model()->headerData(
        index.column(), Qt::Horizontal, (Qt::UserRole + 1)
    );

    // Колонку с состоянием не обрабатываем двойным нажатием
    if (field == "stateDisplay")
        return;

    // Если колонка с исполнителем, тогда переходим к нему
    if (field == "executor")
    {
        auto task = static_cast<Task*>(index.internalPointer());
        if (task->executor() != nullptr)
            emit gotoEmployee(static_cast<Employee*>(task->executor()));

        return;
    }

    editByIndex(index);
}

// =============================================================================
