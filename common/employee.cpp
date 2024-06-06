
#include "employee.h"

// =============================================================================
void Employee::setId(qint32 val) {
    m_id = val;
}

qint32 Employee::id() const {
    return m_id;
}

// =============================================================================
void Employee::setLastName(const QString& val) {
    m_lastName = val;
}

const QString& Employee::lastName() const {
    return m_lastName;
}

// =============================================================================
void Employee::setFirstName(const QString& val) {
    m_firstName = val;
}

const QString& Employee::firstName() const {
    return m_firstName;
}

// =============================================================================
void Employee::setPatronymic(const QString& val) {
    m_patronymic = val;
}

const QString& Employee::patronymic() const {
    return m_patronymic;
}

// =============================================================================
void Employee::setPosition(const QString& val) {
    m_position = val;
}

const QString& Employee::position() const {
    return m_position;
}

// =============================================================================
void Employee::setEmail(const QString& val) {
    m_email = val;
}

const QString& Employee::email() const {
    return m_email;
}

// =============================================================================
void Employee::setPhone(const QString& val) {
    m_phone = val;
}

const QString& Employee::phone() const {
    return m_phone;
}

// =============================================================================
QJsonObject Employee::toJsonObject() const {
    QJsonObject jObj;
    jObj.insert("id", m_id);
    jObj.insert("lastName", m_lastName);
    jObj.insert("firstName", m_firstName);
    jObj.insert("patronymic", m_patronymic);
    jObj.insert("position", m_position);
    jObj.insert("email", m_email);
    jObj.insert("phone", m_phone);
    return jObj;
}

// =============================================================================
Employee Employee::fromJsonObject(const QJsonObject& jObj)
{
    Employee employee;
    employee.setId(jObj["id"].toInt());
    employee.setLastName(jObj["lastName"].toString());
    employee.setFirstName(jObj["firstName"].toString());
    employee.setPatronymic(jObj["patronymic"].toString());
    employee.setPosition(jObj["position"].toString());
    employee.setEmail(jObj["email"].toString());
    employee.setPhone(jObj["phone"].toString());
    return employee;
}

// =============================================================================