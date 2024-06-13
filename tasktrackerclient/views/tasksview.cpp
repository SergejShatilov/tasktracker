
#include "tasksview.h"

#include <QMenu>
#include <QMessageBox>
#include "dialogs/dialogedittask/dialogedittask.h"
#include "views/taskstatedelegate.h"

// =============================================================================
TasksView::TasksView(HttpClient* httpClient,
                     QWidget* parent) :
    QTreeView(parent),
    m_httpClient(httpClient),
    m_tasksModel(new TasksModel(this)),
    m_employeesModel(nullptr)
{
    setModel(m_tasksModel);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QTreeView::customContextMenuRequested,
            this, &TasksView::slotContextMenu);

    connect(this, &QTreeView::doubleClicked,
            this, &TasksView::slotDoubleClicked);

    setColumnHidden(5, true);

    TaskStateDelegate* delegate = new TaskStateDelegate(this);
    setItemDelegateForColumn(2, delegate);
}

// =============================================================================
void TasksView::setEmployeesModel(EmployeesModel* model) {
    m_employeesModel = model;
    m_tasksModel->setEmployeesModel(m_employeesModel);
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
void TasksView::slotCreate(const QModelIndex& index)
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
    dialog->setEmployeesModel(m_employeesModel);

    if (dialog->exec() != QDialog::Accepted)
        return;

    m_tasksModel->addTask(dialog->task(), index);
    expand(index);
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

    setCurrentIndex(QModelIndex());

    // Удаляем задачу и подзадачи в модели, получаем все id для удаления
    const auto ids = m_tasksModel->removeTask(index);

    // Удаляем все задачи с сервера
    for (auto id : ids)
        m_httpClient->deleteTask(id);
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

    auto idTaskParent = m_tasksModel->idByIndex(index.parent());
    auto nameTaskParent = m_tasksModel->nameByIndex(index.parent());

    dialog->setParentTask(idTaskParent, nameTaskParent);
    dialog->setEmployeesModel(m_employeesModel);

    auto obj = m_tasksModel->taskObjectByIndex(index);
    const auto executorId = obj->executorId();
    const auto& executorFullName = m_employeesModel->fullNameById(executorId);
    dialog->setExecutorFullName(executorFullName);
    dialog->setTaskId(obj->id());

    if (dialog->exec() != QDialog::Accepted)
        return;

    obj->setExecutorId(dialog->task().executorId());
}

// =============================================================================
void TasksView::slotDoubleClicked(const QModelIndex& index)
{
    // Для столбца с состоянием игнориуем, т.к. для него отдельный делегат
    if (index.column() == 2)
        return;

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
            slotCreate(index);
        });
    } else {
        menu.addAction(tr("Add..."), this, [this](){
            slotCreate(QModelIndex());
        });
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
