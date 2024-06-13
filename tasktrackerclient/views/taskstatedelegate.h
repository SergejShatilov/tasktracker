
#pragma once

#include <QStyledItemDelegate>

class TaskStateDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TaskStateDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;

    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;

    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

    void paint(QPainter* painter,
               const QStyleOptionViewItem &option,
               const QModelIndex& index) const override;

private:
    QStringList m_listStates;

};
