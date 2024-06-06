
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
