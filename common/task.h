
#pragma once

#include <QString>
#include <QDate>
#include <QJsonObject>

class Task
{
public:
    enum class State
    {
        NotStarted, // Не начата
        Work,       // В работе
        Suspended,  // Приостановлена
        Completed   // Завершено
    };

public:
    Task() = default;

    void setId(qint32 id);
    qint32 id() const;

    void setName(const QString& name);
    const QString& name() const;

    void setState(State state);
    State state() const;

    void setStateString(const QString& stateString);
    QString stateString() const;

    void setExecutor(qint32 executorId);
    qint32 executor() const;

    void setStart(const QDate& start);
    const QDate& start() const;

    void setStartString(const QString& startString);
    QString startString() const;

    void setDuration(qint32 duration);
    qint32 duration() const;

    void setParent(qint32 parentId);
    qint32 parent() const;

    void setDescription(const QString& description);
    const QString& description() const;

    QJsonObject toJsonObject() const;
    static Task fromJsonObject(const QJsonObject& jObj);

protected:
    qint32  m_id;
    QString m_name;
    State   m_state;
    qint32  m_executor;
    QDate   m_start;
    qint32  m_duration;
    qint32  m_parent;
    QString m_description;
};
