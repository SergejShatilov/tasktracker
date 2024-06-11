
#include "tasksview.h"

#include <QMenu>
#include <QMessageBox>
#include "dialogs/dialogedittask/dialogedittask.h"

// =============================================================================
TasksView::TasksView(HttpClient* httpClient,
                     QWidget* parent) :
    QTreeView(parent),
    m_httpClient(httpClient),
    m_tasksModel(new TasksModel(this))
{
    setModel(m_tasksModel);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QTreeView::customContextMenuRequested,
            this, &TasksView::slotContextMenu);

    /*connect(this, &QTreeView::doubleClicked,
            this, &TasksView::slotDoubleClicked);*/
}

// =============================================================================
void TasksView::slotUpdate()
{
    QList<Task> listTasks;
    if (!m_httpClient->getTasks(listTasks))
        return;

    m_tasksModel->loadFromList(listTasks);
}

// =============================================================================
void TasksView::slotCreate()
{
    auto dialog = std::make_unique<DialogEditTask>
    (
        m_httpClient,
        false,
        this
    );

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_tasksModel->addTask(dialog->task());
}

// =============================================================================
void TasksView::slotCreateSub(const QModelIndex& index)
{
    auto dialog = std::make_unique<DialogEditTask>
    (
        m_httpClient,
        false,
        this
    );

    auto idTaskParent = m_tasksModel->idByIndex(index);
    auto nameTaskParent = m_tasksModel->nameByIndex(index);

    dialog->setParentTask(idTaskParent, nameTaskParent);

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_tasksModel->addTask(dialog->task(), index);

    repaint();
}

// =============================================================================
void TasksView::slotDelete()
{
    auto index = currentIndex();

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

    auto id = m_tasksModel->idByIndex(index);

    if (!m_httpClient->deleteTask(id))
        return;

    m_tasksModel->removeTask(index);
}

// =============================================================================
void TasksView::slotEdit(const QModelIndex& index)
{
    auto dialog = std::make_unique<DialogEditTask>
    (
        m_httpClient,
        true,
        this
    );

    dialog->setModel(m_tasksModel);
    dialog->setCurrentModelIndex(index);

    if (dialog->exec() != QDialog::Accepted)
        return;
}

// =============================================================================
void TasksView::slotDoubleClicked(const QModelIndex& index)
{
    slotEdit(index);
}

// =============================================================================
void TasksView::slotContextMenu(const QPoint& pos)
{
    QModelIndex index = indexAt(pos);

    QMenu menu;

    // Если индекс валидный, то предлагаем создать подзадачу
    if (index.isValid()) {
        menu.addAction(tr("Add Subtask..."), this, [this, &index](){
            slotCreateSub(index);
        });
    } else {
        menu.addAction(tr("Add..."), this, &TasksView::slotCreate);
    }

    menu.addSeparator();

    if (index.isValid())
    {
        menu.addAction(tr("Edit..."), this, [this, &index](){
            slotEdit(index);
        });

        menu.addAction(tr("Delete"), this,
                       &TasksView::slotDelete);

        menu.addSeparator();

    }

    menu.addAction(tr("Update"), this, &TasksView::slotUpdate);
    menu.exec(QCursor::pos());
}

// =============================================================================
