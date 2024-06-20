
#include "viewer.h"
#include "ui_viewer.h"

#include <QMenu>

// =============================================================================
Viewer::Viewer(DbRemoteManager* dbManager, QWidget *parent) :
    QWidget(parent),
    m_dbManager(dbManager),
    ui(new Ui::Viewer),
    m_proxyModel(new ViewerProxyModel(this))
{
    ui->setupUi(this);

    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setExpandsOnDoubleClick(false);

    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            this, &Viewer::showCustomContextMenu);

    connect(ui->treeView, &QTreeView::doubleClicked,
            this, &Viewer::doubleClicked);

    connect(ui->comboBoxColumns, &QComboBox::currentTextChanged,
            this, &Viewer::columnSelect);

    connect(ui->lineEditValue, &QLineEdit::textChanged,
            m_proxyModel, &ViewerProxyModel::setFilterValue);
}

// =============================================================================
Viewer::~Viewer()
{
    delete ui;
}

// =============================================================================
void Viewer::setModel(QAbstractItemModel* model)
{
    m_proxyModel->setSourceModel(model);

    for (int i = 0; i < model->columnCount(); ++i)
    {
        bool isVisible = model->headerData(
            i, Qt::Horizontal, (Qt::UserRole)
        ).toBool();

        if (!isVisible)
        {
            ui->treeView->setColumnHidden(i, true);
            continue;
        }

        const QString header = model->headerData(
            i, Qt::Horizontal, Qt::DisplayRole
        ).toString();

        ui->comboBoxColumns->addItem(header, i);
    }
}

// =============================================================================
void Viewer::selectItem(const QModelIndex& index)
{
    auto proxyIndex = m_proxyModel->mapFromSource(index);
    ui->treeView->setCurrentIndex(proxyIndex);
}

// =============================================================================
void Viewer::create()
{
    createByIndex(QModelIndex());
    ui->treeView->expandAll();
}

// =============================================================================
void Viewer::createSub()
{
    createByIndex(currentIndex());
    ui->treeView->expandAll();
}

// =============================================================================
void Viewer::remove()
{
    removeByIndex(currentIndex());
    ui->treeView->expandAll();
}

// =============================================================================
void Viewer::edit()
{
    removeByIndex(currentIndex());
    ui->treeView->expandAll();
}

// =============================================================================
void Viewer::update()
{
    m_dbManager->update();
    ui->treeView->expandAll();
    QWidget::update();
}

// =============================================================================
void Viewer::reset()
{
    ui->lineEditValue->clear();
}

// =============================================================================
QTreeView* Viewer::treeView() const {
    return ui->treeView;
}

// =============================================================================
void Viewer::showCustomContextMenu(const QPoint& pos)
{
    const auto proxyIndex = ui->treeView->indexAt(pos);
    const auto index = m_proxyModel->mapToSource(proxyIndex);

    QMenu menu;
    customContextMenu(&menu, index);

    menu.exec(QCursor::pos());
}

// =============================================================================
void Viewer::doubleClicked(const QModelIndex& proxyIndex)
{
    const auto index = m_proxyModel->mapToSource(proxyIndex);
    itemDoubleClicked(index);
}

// =============================================================================
void Viewer::columnSelect(const QString& str)
{
    Q_UNUSED(str);
    m_proxyModel->setFieldIndex(ui->comboBoxColumns->currentData().toInt());
    reset();
}

// =============================================================================
QModelIndex Viewer::currentIndex() const
{
    const auto proxyIndex = ui->treeView->currentIndex();
    return m_proxyModel->mapToSource(proxyIndex);
}

// =============================================================================
