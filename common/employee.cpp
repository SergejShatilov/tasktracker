
#include "employee.h"

#include <QJsonDocument>
#include <QJsonArray>

// =============================================================================
Employee::Employee(QObject* parent) :
    QObject(parent),
    m_id(0)
{
}

// =============================================================================
bool Employee::isValid() const {
    return (m_id != 0);
}

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
QString Employee::fullName() const
{
    return QString("%1 %2 %3")
        .arg(m_lastName)
        .arg(m_firstName)
        .arg(m_patronymic);
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
void Employee::addTask(QObject* task) {
    m_tasks.append(task);
}

// =============================================================================
void Employee::removeTask(QObject* task) {
    m_tasks.removeOne(task);
}

// =============================================================================
const QList<QObject*>& Employee::tasks() const {
    return m_tasks;
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
void Employee::fromJson(const QByteArray& data)
{
    QJsonObject jObj = QJsonDocument::fromJson(data).object();
    setId(jObj["id"].toInt());
    setLastName(jObj["lastName"].toString());
    setFirstName(jObj["firstName"].toString());
    setPatronymic(jObj["patronymic"].toString());
    setPosition(jObj["position"].toString());
    setEmail(jObj["email"].toString());
    setPhone(jObj["phone"].toString());
}

// =============================================================================
QByteArray Employee::toJson() const {
    return QJsonDocument(toJsonObject()).toJson(QJsonDocument::Compact);
}

// =============================================================================
Employee* Employee::createFromJsonObject(
    const QJsonObject& jObj,
    QObject* parent)
{
    auto employee = new Employee(parent);

    employee->setId(jObj["id"].toInt());
    employee->setLastName(jObj["lastName"].toString());
    employee->setFirstName(jObj["firstName"].toString());
    employee->setPatronymic(jObj["patronymic"].toString());
    employee->setPosition(jObj["position"].toString());
    employee->setEmail(jObj["email"].toString());
    employee->setPhone(jObj["phone"].toString());

    return employee;
}

// =============================================================================
Employee* Employee::createFromJson(
    const QByteArray& data,
    QObject* parent)
{
    return Employee::createFromJsonObject(
        QJsonDocument::fromJson(data).object(),
        parent
    );
}

// =============================================================================
QList<Employee*> Employee::createListFromJson(
    const QByteArray& data,
    QObject* parent)
{
    QList<Employee*> list;

    QJsonObject jObj = QJsonDocument::fromJson(data).object();

    QJsonArray jArray = jObj["employees"].toArray();
    for (const auto& jEmployee : jArray) {
        list.append(Employee::createFromJsonObject(jEmployee.toObject(), parent));
    }

    return list;
}

// =============================================================================
QByteArray Employee::listToJson(
    const QList<QSharedPointer<Employee>>& listEmployees)
{
    QJsonArray jArray;
    for (const auto& employee : listEmployees) {
        jArray.append(employee->toJsonObject());
    }

    QJsonObject jObj;
    jObj.insert("employees", QJsonValue(jArray));

    return QJsonDocument(jObj).toJson(QJsonDocument::JsonFormat::Compact);
}

// =============================================================================
QDebug operator<<(QDebug d, const Employee* employee)
{
    d << "Employee:\r\n";
    d << "id:" << employee->id() << "\r\n";
    d << "lastName:" << employee->lastName() << "\r\n";
    d << "firstName:" << employee->firstName() << "\r\n";
    d << "patronymic:" << employee->patronymic() << "\r\n";
    d << "position:" << employee->position() << "\r\n";
    d << "email:" << employee->email() << "\r\n";
    d << "phone:" << employee->phone() << "\r\n";
    return d;
}

// =============================================================================
