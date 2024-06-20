
#include "tasksdelegate.h"

#include <QTreeView>

// =============================================================================
TasksDelegate::TasksDelegate(TasksModel* tasksModel, QObject* parent) :
    QStyledItemDelegate(parent),
    m_tasksModel(tasksModel),
    m_proxyModel(new TaskExecutorFilterModel(this))
{
    m_proxyModel->setSourceModel(m_tasksModel);
}

// =============================================================================
QWidget* TasksDelegate::createEditor(
    QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QTreeView* treeView = new QTreeView(parent);
    treeView->setAutoFillBackground(true);

    if (index.isValid())
    {
        EmployeeObject* employeeObject = static_cast<EmployeeObject*>
            (index.internalPointer());

        m_proxyModel->setExecutorId(employeeObject->id());
        treeView->setModel(m_proxyModel);

        treeView->setColumnHidden(1, true);
        treeView->setColumnHidden(2, true);
        treeView->setColumnHidden(3, true);
        treeView->setColumnHidden(5, true);
        treeView->setColumnHidden(6, true);
    }

    return treeView;
}

// =============================================================================
void TasksDelegate::setEditorData(
    QWidget* editor,
    const QModelIndex& index) const
{
    //QListWidget* comboBox = static_cast<QComboBox*>(editor);
    //comboBox->setCurrentText("Suspended");
}

// =============================================================================
void TasksDelegate::setModelData(
    QWidget* editor,
    QAbstractItemModel* model,
    const QModelIndex& index) const
{
    /*QComboBox* comboBox = static_cast<QComboBox*>(editor);

    const QStringList list {"NotStarted", "Work", "Suspended", "Completed"};

    const auto currentIndex = comboBox->currentIndex();
    if (currentIndex >= list.size())
        return;

    model->setData(index, list.at(currentIndex));*/
}

// =============================================================================
void TasksDelegate::updateEditorGeometry(
    QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect.x(), option.rect.y(),
                        option.rect.width(), 200);
}

// =============================================================================
void TasksDelegate::paint(
    QPainter* painter,
    const QStyleOptionViewItem &option,
    const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
};

// =============================================================================
