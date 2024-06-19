
#include "task.h"

#include <QHash>
#include <QMap>
#include <QJsonDocument>
#include <QJsonArray>

// =============================================================================
Task::Task(QObject* parent) :
    QObject(parent),
    m_id(0),
    m_name(QString("Undefined")),
    m_state(State::NotStarted),
    m_executorId(0),
    m_parentId(0),
    m_executor(nullptr)
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
        {"NotStarted", State::NotStarted},
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
void Task::setDeadline(const QDate& deadline) {
    m_deadline = deadline;
}

const QDate& Task::deadline() const {
    return m_deadline;
}

// =============================================================================
void Task::setDeadlineString(const QString& startString) {
    m_deadline = QDate::fromString(startString, Qt::DateFormat::ISODate);
}

QString Task::deadlineString() const {
    return m_deadline.toString(Qt::DateFormat::ISODate);
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
void Task::setExecutor(QObject* executor) {
    m_executor = executor;
}

QObject* Task::executor() const {
    return m_executor;
}

// =============================================================================
QJsonObject Task::toJsonObject() const {
    QJsonObject jObj;
    jObj.insert("id", m_id);
    jObj.insert("name", m_name);
    jObj.insert("state", stateString());
    jObj.insert("executor", m_executorId);
    jObj.insert("deadline", deadlineString());
    jObj.insert("parent", m_parentId);
    jObj.insert("description", m_description);
    return jObj;
}

// =============================================================================
void Task::fromJson(const QByteArray& data)
{
    QJsonObject jObj = QJsonDocument::fromJson(data).object();
    setId(jObj["id"].toInt());
    setName(jObj["name"].toString());
    setStateString(jObj["state"].toString());
    setExecutorId(jObj["executor"].toInt());
    setDeadlineString(jObj["deadline"].toString());
    setParentId(jObj["parent"].toInt());
    setDescription(jObj["description"].toString());
}

// =============================================================================
QByteArray Task::toJson() const {
    return QJsonDocument(toJsonObject()).toJson(QJsonDocument::Compact);
}

// =============================================================================
Task* Task::createFromJsonObject(const QJsonObject& jObj, QObject* parent)
{
    auto task = new Task(parent);

    task->setId(jObj["id"].toInt());
    task->setName(jObj["name"].toString());
    task->setStateString(jObj["state"].toString());
    task->setExecutorId(jObj["executor"].toInt());
    task->setDeadlineString(jObj["deadline"].toString());
    task->setParentId(jObj["parent"].toInt());
    task->setDescription(jObj["description"].toString());

    return task;
}

// =============================================================================
Task* Task::createFromJson(const QByteArray& data, QObject* parent)
{
    return Task::createFromJsonObject(
        QJsonDocument::fromJson(data).object(),
        parent
    );
}

// =============================================================================
QList<Task*> Task::createListFromJson(const QByteArray& data, QObject* parent)
{
    QList<Task*> list;

    QJsonObject jObj = QJsonDocument::fromJson(data).object();

    QJsonArray jArray = jObj["tasks"].toArray();
    for (const auto& jTask : jArray) {
        list.append(Task::createFromJsonObject(jTask.toObject(), parent));
    }

    return list;
}

// =============================================================================
QByteArray Task::listToJson(const QList<QSharedPointer<Task>>& listTasks)
{
    QJsonArray jArray;
    for (const auto& employee : listTasks) {
        jArray.append(employee->toJsonObject());
    }

    QJsonObject jObj;
    jObj.insert("tasks", QJsonValue(jArray));

    return QJsonDocument(jObj).toJson(QJsonDocument::Compact);
}

// =============================================================================
QDebug operator<<(QDebug d, const Task* task)
{
    d << "Task:\r\n";
    d << "id:" << task->id() << "\r\n";
    d << "name:" << task->name() << "\r\n";
    d << "state:" << task->stateString() << "\r\n";
    d << "parentId:" << task->parentId() << "\r\n";
    d << "executorId:" << task->executorId() << "\r\n";
    d << "deadline:" << task->deadlineString() << "\r\n";
    d << "description:" << task->description() << "\r\n";

    return d;
}

// =============================================================================
