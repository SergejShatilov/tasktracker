
#include "task.h"

#include <QHash>
#include <QMap>

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
        throw std::runtime_error("state is invalid");
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
        throw std::runtime_error("state is invalid");

    return it.value();
}

// =============================================================================
void Task::setExecutor(qint32 executorId) {
    m_executor = executorId;
}

qint32 Task::executor() const {
    return m_executor;
}

// =============================================================================
void Task::setStart(const QDate& start) {
    m_start = start;
}

const QDate& Task::start() const {
    return m_start;
}

// =============================================================================
void Task::setDuration(qint32 duration) {
    m_duration = duration;
}

qint32 Task::duration() const {
    return m_duration;
}

// =============================================================================
void Task::setParent(qint32 parentId) {
    m_parent = parentId;
}

qint32 Task::parent() const {
    return m_parent;
}

// =============================================================================
void Task::setDescription(const QString& description) {
    m_description = description;
}

const QString& Task::description() const {
    return m_description;
}

// =============================================================================
QJsonObject Task::toJsonObject() const {
    QJsonObject jObj;
    jObj.insert("id", m_id);
    jObj.insert("name", m_name);
    jObj.insert("state", stateString());
    jObj.insert("executor", m_executor);
    jObj.insert("start", m_start.toString());
    jObj.insert("duration", m_duration);
    jObj.insert("parent", m_parent);
    jObj.insert("description", m_description);
    return jObj;
}

// =============================================================================
Task Task::fromJsonObject(const QJsonObject& jObj)
{
    Task task;
    task.setId(jObj["id"].toInt());
    task.setName(jObj["name"].toString());
    task.setStateString(jObj["state"].toString());
    task.setExecutor(jObj["executor"].toInt());
    task.setStart(QDate().fromString(jObj["start"].toString()));
    task.setDuration(jObj["duration"].toInt());
    task.setParent(jObj["parent"].toInt());
    task.setDescription(jObj["description"].toString());
    return task;
}

// =============================================================================
