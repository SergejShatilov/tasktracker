
#include "connectdialog.h"
#include "ui_connectdialog.h"

// =============================================================================
ConnectDialog::ConnectDialog(DbRemoteManager* dbManager,
                             QWidget *parent,
                             bool isNewDb) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    m_dbManager(dbManager),
    m_funcHandler(nullptr)
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->pushButtonOk, &QPushButton::clicked,
            this, &ConnectDialog::clickedOk);

    connect(ui->lineEditNameDb, &QLineEdit::textChanged,
            this, &ConnectDialog::changedName);

    if (isNewDb)
    {
        setWindowTitle(tr("Create new db..."));
        m_funcHandler = [this](const QString& dbName) {
            return m_dbManager->createDb(dbName);
        };
    }
    else
    {
        setWindowTitle(tr("Connect to db..."));
        m_funcHandler = [this](const QString& dbName) {
            return m_dbManager->checkExistDb(dbName);
        };
    }
}

// =============================================================================
ConnectDialog::~ConnectDialog()
{
    delete ui;
}

// =============================================================================
QString ConnectDialog::dbName() const {
    return ui->lineEditNameDb->text();
}

// =============================================================================
void ConnectDialog::clickedOk()
{
    if (m_funcHandler == nullptr)
        return;

    const QString dbName = ui->lineEditNameDb->text();

    ui->pushButtonOk->setEnabled(false);

    if (m_funcHandler(dbName))
    {
        m_dbManager->setDbName(dbName);
        accept();
        return;
    }

    ui->pushButtonOk->setEnabled(true);
}

// =============================================================================
void ConnectDialog::changedName(const QString &name)
{
    ui->pushButtonOk->setEnabled(!name.isEmpty());
}

// =============================================================================
