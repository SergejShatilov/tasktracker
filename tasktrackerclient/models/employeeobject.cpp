
#include "employeeobject.h"

// =============================================================================
EmployeeObject::EmployeeObject(QObject* parent) :
    QObject(parent),
    Employee()
{

}

// =============================================================================
EmployeeObject::EmployeeObject(const Employee& employee,
                               QObject* parent) :
    QObject(parent),
    Employee(employee)
{

}

// =============================================================================
QString EmployeeObject::fullName() const
{
    return QString("%1 %2 %3")
        .arg(m_lastName)
        .arg(m_firstName)
        .arg(m_patronymic);
}

// =============================================================================
