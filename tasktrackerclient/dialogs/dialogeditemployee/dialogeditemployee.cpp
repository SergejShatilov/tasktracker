
#include "dialogeditemployee.h"
#include "ui_dialogeditemployee.h"

#include <QModelIndex>

// =============================================================================
DialogEditEmployee::DialogEditEmployee(HttpClient* httpClient,
                                       bool isEdit,
                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditEmployee),
    m_httpClient(httpClient),
    m_mapper(new QDataWidgetMapper(this)),
    m_employee(Employee())
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

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

    if (isEdit) {
        connect(ui->pushButtonOk, &QPushButton::clicked,
                this, &DialogEditEmployee::submit);
    } else {
        connect(ui->pushButtonOk, &QPushButton::clicked,
                this, &DialogEditEmployee::createNewEmployee);
    }
}

// =============================================================================
DialogEditEmployee::~DialogEditEmployee()
{
    delete ui;
}

// =============================================================================
void DialogEditEmployee::setModel(QAbstractItemModel* model)
{
    m_mapper->setSubmitPolicy(QDataWidgetMapper::SubmitPolicy::ManualSubmit);
    m_mapper->setModel(model);
    m_mapper->addMapping(ui->lineEditLastName, 1);
    m_mapper->addMapping(ui->lineEditFirstName, 2);
    m_mapper->addMapping(ui->lineEditPatronymic, 3);
    m_mapper->addMapping(ui->lineEditPosition, 5);
    m_mapper->addMapping(ui->lineEditEmail, 6);
    m_mapper->addMapping(ui->lineEditPhone, 7);
}

// =============================================================================
void DialogEditEmployee::setCurrentModelIndex(const QModelIndex& index)
{
    if (m_mapper->model() == nullptr)
        return;

    QModelIndex idIndex = m_mapper->model()->index(index.row(), 0);

    m_employee.setId(m_mapper->model()->data(idIndex).toInt());

    m_mapper->setCurrentModelIndex(index);
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
void DialogEditEmployee::submit()
{
    m_employee.setLastName(ui->lineEditLastName->text());
    m_employee.setFirstName(ui->lineEditFirstName->text());
    m_employee.setPatronymic(ui->lineEditPatronymic->text());
    m_employee.setPosition(ui->lineEditPosition->text());
    m_employee.setEmail(ui->lineEditEmail->text());
    m_employee.setPhone(ui->lineEditPhone->text());

    ui->pushButtonOk->setEnabled(false);
    if (!m_httpClient->changeEmployee(m_employee.id(), m_employee)) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    m_mapper->submit();
    accept();
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

    Employee employee = m_httpClient->addEmployee(m_employee);

    if (!employee.isValid()) {
        ui->pushButtonOk->setEnabled(true);
        return;
    }

    m_employee = employee;

    accept();
}

// =============================================================================
