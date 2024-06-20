
#include "statedelegate.h"

#include <QComboBox>

// =============================================================================
StateDelegate::StateDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{
}

// =============================================================================
QWidget* StateDelegate::createEditor(
    QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox* comboBox = new QComboBox(parent);
    comboBox->setAutoFillBackground(true);

    comboBox->addItem(tr("Not Started"));
    comboBox->addItem(tr("Work"));
    comboBox->addItem(tr("Suspended"));
    comboBox->addItem(tr("Completed"));

    return comboBox;
}

// =============================================================================
void StateDelegate::setEditorData(
    QWidget* editor,
    const QModelIndex& index) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
}

// =============================================================================
void StateDelegate::setModelData(
    QWidget* editor,
    QAbstractItemModel* model,
    const QModelIndex& index) const
{
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentIndex());
}

// =============================================================================
void StateDelegate::updateEditorGeometry(
    QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}

// =============================================================================
