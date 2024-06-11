
#pragma once

#include <QString>
#include <QJsonObject>
#include <QDebug>

class Employee
{
public:
    Employee();

    bool isValid() const;

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

    static Employee fromJsonObject(const QJsonObject& jObj);
    QJsonObject toJsonObject() const;

    static Employee fromJson(const QByteArray& data);
    QByteArray toJson() const;

protected:
    qint32  m_id;
    QString m_lastName;
    QString m_firstName;
    QString m_patronymic;
    QString m_position;
    QString m_email;
    QString m_phone;
};

QDebug operator<<(QDebug d, const Employee& employee);
