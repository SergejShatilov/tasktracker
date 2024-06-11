
#pragma once

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QDebug>

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
    Task();

    bool isValid() const;

    void setId(qint32 id);
    qint32 id() const;

    void setName(const QString& name);
    const QString& name() const;

    void setState(State state);
    State state() const;

    void setStateString(const QString& stateString);
    QString stateString() const;

    void setExecutorId(qint32 executorId);
    qint32 executorId() const;

    void setDeadline(const QDate& deadline);
    const QDate& deadline() const;

    void setDeadlineString(const QString& deadlineString);
    QString deadlineString() const;

    void setParentId(qint32 parentId);
    qint32 parentId() const;

    void setDescription(const QString& description);
    const QString& description() const;

    static Task fromJsonObject(const QJsonObject& jObj);
    QJsonObject toJsonObject() const;

    static Task fromJson(const QByteArray& data);
    QByteArray toJson() const;

protected:
    qint32  m_id;
    QString m_name;
    State   m_state;
    qint32  m_executorId;
    QDate   m_deadline;
    qint32  m_parentId;
    QString m_description;
};

QDebug operator<<(QDebug d, const Task& task);
