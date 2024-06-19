
#include "employeedialog.h"
#include "ui_employeedialog.h"

// =============================================================================
EmployeeDialog::EmployeeDialog(DbRemoteManager* dbManager,
                               QWidget *parent,
                               Employee* employee) :
    QDialog(parent),
    ui(new Ui::EmployeeDialog),
    m_dbManager(dbManager),
    m_employee(employee)
{
    ui->setupUi(this);

    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    connect(ui->pushButtonOk, &QPushButton::clicked,
            this, &EmployeeDialog::clickedOk);

    connect(ui->lineEditLastName, &QLineEdit::textChanged,
            this, &EmployeeDialog::changed);

    connect(ui->lineEditFirstName, &QLineEdit::textChanged,
            this, &EmployeeDialog::changed);

    // Если объект не существует, значит создаем нового сотрудника,
    // Иначе редактируем уже существующего
    if (m_employee == nullptr)
    {
        m_employee = new Employee(this->parent());
        setWindowTitle(tr("Create new employee..."));
        m_funcHandler = [this](Employee* employee) {
            return m_dbManager->addEmployee(employee);
        };
    }
    else
    {
        setWindowTitle(tr("Edit employee..."));
        ui->lineEditLastName->setText(m_employee->lastName());
        ui->lineEditFirstName->setText(m_employee->firstName());
        ui->lineEditPatronymic->setText(m_employee->patronymic());
        ui->lineEditPosition->setText(m_employee->position());
        ui->lineEditEmail->setText(m_employee->email());
        ui->lineEditPhone->setText(m_employee->phone());

        m_funcHandler = [this](Employee* employee) {
            return m_dbManager->changeEmployee(employee);
        };
    }
}

// =============================================================================
EmployeeDialog::~EmployeeDialog()
{
    delete ui;
}

// =============================================================================
const Employee* EmployeeDialog::employee() const {
    return m_employee;
}

// =============================================================================
void EmployeeDialog::clickedOk()
{
    if (m_funcHandler == nullptr)
        return;

    m_employee->setLastName(ui->lineEditLastName->text());
    m_employee->setFirstName(ui->lineEditFirstName->text());
    m_employee->setPatronymic(ui->lineEditPatronymic->text());
    m_employee->setPosition(ui->lineEditPosition->text());
    m_employee->setEmail(ui->lineEditEmail->text());
    m_employee->setPhone(ui->lineEditPhone->text());

    ui->pushButtonOk->setEnabled(false);

    if (m_funcHandler(m_employee))
    {
        accept();
        return;
    }

    ui->pushButtonOk->setEnabled(true);
}

// =============================================================================
void EmployeeDialog::changed(const QString& str)
{
    Q_UNUSED(str);
    ui->pushButtonOk->setEnabled(!ui->lineEditLastName->text().isEmpty() &&
                                 !ui->lineEditFirstName->text().isEmpty());
}

// =============================================================================
