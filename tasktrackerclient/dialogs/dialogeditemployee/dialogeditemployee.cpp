
#include "dialogeditemployee.h"
#include "ui_dialogeditemployee.h"

// =============================================================================
DialogEditEmployee::DialogEditEmployee(std::shared_ptr<HttpClient> httpClient,
                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditEmployee),
    m_httpClient(httpClient)
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->pushButtonOk, &QPushButton::clicked,
            this, &DialogEditEmployee::createNewEmployee);

    connect(ui->lineEditLastName, &QLineEdit::textChanged,
            this, &DialogEditEmployee::changed);

    connect(ui->lineEditFirstName, &QLineEdit::textChanged,
            this, &DialogEditEmployee::changed);

    connect(ui->lineEditPatronymic, &QLineEdit::textChanged,
            this, &DialogEditEmployee::changed);

    connect(ui->lineEditPosition, &QLineEdit::textChanged,
            this, &DialogEditEmployee::changed);

    connect(ui->lineEditEmail, &QLineEdit::textChanged,
            this, &DialogEditEmployee::changed);

    connect(ui->lineEditPhone, &QLineEdit::textChanged,
            this, &DialogEditEmployee::changed);
}

// =============================================================================
DialogEditEmployee::~DialogEditEmployee()
{
    delete ui;
}

// =============================================================================
void DialogEditEmployee::setLastName(const QString& lastName) {
    ui->lineEditLastName->setText(lastName);
}

void DialogEditEmployee::setFirstName(const QString& firstName) {
    ui->lineEditFirstName->setText(firstName);
}

void DialogEditEmployee::setPatronymic(const QString &patronymic) {
    ui->lineEditPatronymic->setText(patronymic);
}

void DialogEditEmployee::setPosition(const QString& position) {
    ui->lineEditPosition->setText(position);
}

void DialogEditEmployee::setEmail(const QString& email) {
    ui->lineEditEmail->setText(email);
}

void DialogEditEmployee::setPhone(const QString& phone) {
    ui->lineEditPhone->setText(phone);
}

// =============================================================================
const Employee& DialogEditEmployee::employee() const {
    return m_employee;
}

// =============================================================================
void DialogEditEmployee::changed(const QString &)
{
    ui->pushButtonOk->setEnabled(!ui->lineEditLastName->text().isEmpty() &&
                                 !ui->lineEditFirstName->text().isEmpty() &&
                                 !ui->lineEditPatronymic->text().isEmpty() &&
                                 !ui->lineEditPosition->text().isEmpty() &&
                                 !ui->lineEditEmail->text().isEmpty() &&
                                 !ui->lineEditPhone->text().isEmpty());
}

// =============================================================================
void DialogEditEmployee::createNewEmployee()
{
    m_employee.setLastName(ui->lineEditLastName->text());
    m_employee.setFirstName(ui->lineEditFirstName->text());
    m_employee.setPatronymic(ui->lineEditPatronymic->text());
    m_employee.setPosition(ui->lineEditPosition->text());
    m_employee.setEmail(ui->lineEditEmail->text());
    m_employee.setPhone(ui->lineEditPhone->text());

    ui->pushButtonOk->setEnabled(false);
    if (!m_httpClient->addEmployee(m_employee)) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    accept();
}

// =============================================================================
