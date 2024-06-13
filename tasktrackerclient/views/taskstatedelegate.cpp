
#include "taskstatedelegate.h"

#include <QComboBox>

// =============================================================================
TaskStateDelegate::TaskStateDelegate(QObject* parent) :
    QStyledItemDelegate(parent),
    m_listStates({tr("Not Started"), tr("Work"),
                  tr("Suspended"), tr("Completed")})
{

}

// =============================================================================
QWidget* TaskStateDelegate::createEditor(
    QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox* comboBox = new QComboBox(parent);
    comboBox->setAutoFillBackground(true);
    comboBox->addItems(m_listStates);

    return comboBox;
}

// =============================================================================
void TaskStateDelegate::setEditorData(
    QWidget* editor,
    const QModelIndex& index) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(editor);

    static QHash<QString, int> const tableStates = {
        {"NotStarted",  0},
        {"Work",        1},
        {"Suspended",   2},
        {"Completed",   3}
    };

    auto it = tableStates.find(index.data().toString());
    if (it == tableStates.end())
        return;

    comboBox->setCurrentText(m_listStates.at(*it));
}

// =============================================================================
void TaskStateDelegate::setModelData(
    QWidget* editor,
    QAbstractItemModel* model,
    const QModelIndex& index) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(editor);

    const QStringList list {"NotStarted", "Work", "Suspended", "Completed"};

    const auto currentIndex = comboBox->currentIndex();
    if (currentIndex >= list.size())
        return;

    model->setData(index, list.at(currentIndex));
}

// =============================================================================
void TaskStateDelegate::updateEditorGeometry(
    QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}

// =============================================================================
void TaskStateDelegate::paint(
    QPainter* painter,
    const QStyleOptionViewItem &option,
    const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
};

// =============================================================================
