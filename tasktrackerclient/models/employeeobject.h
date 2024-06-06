
#pragma once

#include <QObject>
#include "employee.h"

class EmployeeObject : public QObject, public Employee
{
    Q_OBJECT

public:
    explicit EmployeeObject(QObject* parent = nullptr);
    explicit EmployeeObject(const Employee& employee,
                            QObject* parent = nullptr);

    QString fullName() const;

public:
    Q_PROPERTY(qint32 id READ id WRITE setId);
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName);
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName);
    Q_PROPERTY(QString patronymic READ patronymic WRITE setPatronymic);
    Q_PROPERTY(QString position READ position WRITE setPosition);
    Q_PROPERTY(QString email READ email WRITE setEmail);
    Q_PROPERTY(QString phone READ phone WRITE setPhone);
    Q_PROPERTY(QString fullName READ fullName);
};
