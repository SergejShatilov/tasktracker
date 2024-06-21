
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QMessageBox>

// =============================================================================
SettingsDialog::SettingsDialog(Settings* settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    ui->lineEditHostName->setText(m_settings->hostName());
    ui->spinBoxPort->setValue(m_settings->port());

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &SettingsDialog::reject);

    connect(ui->pushButtonOk, &QPushButton::clicked,
            this, &SettingsDialog::pressOk);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

// =============================================================================
void SettingsDialog::pressOk()
{
    QMessageBox::StandardButton reply = QMessageBox::question
    (
        this,
        QString(),
        tr("To apply the settings, you need to restart the application!"),
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (reply != QMessageBox::Ok)
        return;

    m_settings->setHostName(ui->lineEditHostName->text());
    m_settings->setPort(ui->spinBoxPort->value());
    m_settings->commit();

    accept();
}

// =============================================================================
