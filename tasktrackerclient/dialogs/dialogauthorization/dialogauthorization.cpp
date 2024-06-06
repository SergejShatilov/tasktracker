#include "dialogauthorization.h"
#include "ui_dialogauthorization.h"

DialogAuthorization::DialogAuthorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuthorization)
{
    ui->setupUi(this);

    connect(ui->pushButtonOk, &QPushButton::clicked, this, &QDialog::accept);
}

DialogAuthorization::~DialogAuthorization()
{
    delete ui;
}

QString DialogAuthorization::userName() const {
    return ui->lineEditUserName->text();
}

QString DialogAuthorization::password() const {
    return ui->lineEditPassword->text();
}
