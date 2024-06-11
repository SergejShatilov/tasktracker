
#include "task.h"

#include <QHash>
#include <QMap>

// =============================================================================
Task::Task() :
    m_id(0),
    m_name(QString("Undefined")),
    m_state(State::NotStarted),
    m_executorId(0),
    m_duration(0),
    m_parentId(0),
    m_description(QString())
{
}

// =============================================================================
bool Task::isValid() const {
    return (m_id != 0);
}

// =============================================================================
void Task::setId(qint32 val) {
    m_id = val;
}

qint32 Task::id() const {
    return m_id;
}

// =============================================================================
void Task::setName(const QString& val) {
    m_name = val;
}

const QString& Task::name() const {
    return m_name;
}

// =============================================================================
void Task::setState(State state) {
    m_state = state;
}

Task::State Task::state() const {
    return m_state;
}

// =============================================================================
void Task::setStateString(const QString& stateString)
{
    static QHash<QString, State> const tableStates = {
        {"NotStarted",  State::NotStarted},
        {"Work",        State::Work},
        {"Suspended",   State::Suspended},
        {"Completed",   State::Completed}
    };

    auto it = tableStates.find(stateString);
    if (it != tableStates.end()) {
        m_state = it.value();
    } else {
        m_state = State::NotStarted;
    }
}

// =============================================================================
QString Task::stateString() const
{
    static QMap<State, QString> const tableStates {
        {State::NotStarted, "NotStarted"},
        {State::Work,       "Work"},
        {State::Suspended,  "Suspended"},
        {State::Completed,  "Completed"}
    };

    const auto it = tableStates.find(m_state);
    if (it == tableStates.end())
        return QString("NotStarted");

    return it.value();
}

// =============================================================================
void Task::setExecutorId(qint32 executorId) {
    m_executorId = executorId;
}

qint32 Task::executorId() const {
    return m_executorId;
}

// =============================================================================
void Task::setStart(const QDate& start) {
    m_start = start;
}

const QDate& Task::start() const {
    return m_start;
}

// =============================================================================
void Task::setStartString(const QString& startString)
{
    m_start = QDate::fromString(startString, Qt::DateFormat::ISODate);
}

// =============================================================================
QString Task::startString() const
{
    return m_start.toString(Qt::DateFormat::ISODate);
}

// =============================================================================
void Task::setDuration(qint32 duration) {
    m_duration = duration;
}

qint32 Task::duration() const {
    return m_duration;
}

// =============================================================================
void Task::setParentId(qint32 parentId) {
    m_parentId = parentId;
}

qint32 Task::parentId() const {
    return m_parentId;
}

// =============================================================================
void Task::setDescription(const QString& description) {
    m_description = description;
}

const QString& Task::description() const {
    return m_description;
}

// =============================================================================
Task Task::fromJsonObject(const QJsonObject& jObj)
{
    Task task;
    task.setId(jObj["id"].toInt());
    task.setName(jObj["name"].toString());
    task.setStateString(jObj["state"].toString());
    task.setExecutorId(jObj["executor"].toInt());
    task.setStartString(jObj["start"].toString());
    task.setDuration(jObj["duration"].toInt());
    task.setParentId(jObj["parent"].toInt());
    task.setDescription(jObj["description"].toString());

    return task;
}

// =============================================================================
QJsonObject Task::toJsonObject() const {
    QJsonObject jObj;
    jObj.insert("id", m_id);
    jObj.insert("name", m_name);
    jObj.insert("state", stateString());
    jObj.insert("executor", m_executorId);
    jObj.insert("start", startString());
    jObj.insert("duration", m_duration);
    jObj.insert("parent", m_parentId);
    jObj.insert("description", m_description);
    return jObj;
}

// =============================================================================
Task Task::fromJson(const QByteArray& data) {
    return Task::fromJsonObject(QJsonDocument::fromJson(data).object());
}

// =============================================================================
QByteArray Task::toJson() const {
    return QJsonDocument(toJsonObject()).toJson(QJsonDocument::Compact);
}

// =============================================================================
QDebug operator<<(QDebug d, const Task& task)
{
    d << "id:" << task.id() << "\r\n";
    d << "name:" << task.name() << "\r\n";
    d << "parentId:" << task.parentId() << "\r\n";

    return d;
}

// =============================================================================
