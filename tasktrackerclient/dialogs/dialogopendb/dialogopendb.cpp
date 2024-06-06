
#include "dialogopendb.h"
#include "ui_dialogopendb.h"

// =============================================================================
DialogOpenDb::DialogOpenDb(std::shared_ptr<HttpClient> httpClient,
                           bool isNewDb,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpenDb),
    m_httpClient(httpClient)
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->lineEditNameDb, &QLineEdit::textChanged,
            this, &DialogOpenDb::changedName);

    if (isNewDb) {
        connect(ui->pushButtonOk, &QPushButton::clicked,
                this, &DialogOpenDb::createNewDb);
    } else {
        connect(ui->pushButtonOk, &QPushButton::clicked,
                this, &DialogOpenDb::openDb);
    }
}

// =============================================================================
DialogOpenDb::~DialogOpenDb()
{
    delete ui;
}

// =============================================================================
void DialogOpenDb::setDbName(const QString& dbname)
{
    ui->lineEditNameDb->setText(dbname);
}

// =============================================================================
QString DialogOpenDb::dbName() const
{
    return ui->lineEditNameDb->text();
}

// =============================================================================
void DialogOpenDb::createNewDb()
{
    ui->pushButtonOk->setEnabled(false);
    if (!m_httpClient->createDb(ui->lineEditNameDb->text())) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    accept();
}

// =============================================================================
void DialogOpenDb::openDb()
{
    ui->pushButtonOk->setEnabled(false);
    if (!m_httpClient->openDb(ui->lineEditNameDb->text())) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    accept();
}

// =============================================================================
void DialogOpenDb::changedName(const QString &name)
{
    ui->pushButtonOk->setEnabled(!name.isEmpty());
}

// =============================================================================
