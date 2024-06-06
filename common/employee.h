
#pragma once

#include <QString>
#include <QJsonObject>

class Employee
{
public:
    Employee() = default;

    void setId(qint32 val);
    qint32 id() const;

    void setLastName(const QString& val);
    const QString& lastName() const;

    void setFirstName(const QString& val);
    const QString& firstName() const;

    void setPatronymic(const QString& val);
    const QString& patronymic() const;

    void setPosition(const QString& val);
    const QString& position() const;

    void setEmail(const QString& val);
    const QString& email() const;

    void setPhone(const QString& val);
    const QString& phone() const;

    QJsonObject toJsonObject() const;
    static Employee fromJsonObject(const QJsonObject& jObj);

protected:
    qint32  m_id;
    QString m_lastName;
    QString m_firstName;
    QString m_patronymic;
    QString m_position;
    QString m_email;
    QString m_phone;
};
