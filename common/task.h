
#pragma once

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QSharedPointer>
#include <QDebug>

class Task : public QObject
{
    Q_OBJECT

public:
    enum class State
    {
        NotStarted, // Не начата
        Work,       // В работе
        Suspended,  // Приостановлена
        Completed   // Завершено
    };

public:
    explicit Task(QObject* parent = nullptr);

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

    void setExecutor(QObject* executor);
    QObject* executor() const;

    // ----------------------------------------
    QJsonObject toJsonObject() const;

    void fromJson(const QByteArray& data);
    QByteArray toJson() const;

    static Task* createFromJsonObject(
        const QJsonObject& jObj,
        QObject* parent = nullptr
    );

    static Task* createFromJson(
        const QByteArray& data,
        QObject* parent = nullptr
    );

    static QList<Task*> createListFromJson(
        const QByteArray& data,
        QObject* parent = nullptr
    );

    static QByteArray listToJson(
        const QList<QSharedPointer<Task>>& listTasks
    );

public:
    Q_PROPERTY(qint32 id READ id WRITE setId);
    Q_PROPERTY(QString name READ name WRITE setName);
    Q_PROPERTY(QString state READ stateString WRITE setStateString);
    Q_PROPERTY(qint32 executorId READ executorId WRITE setExecutorId);
    Q_PROPERTY(QDate deadline READ deadline WRITE setDeadline);
    Q_PROPERTY(qint32 parentId READ parentId WRITE setParentId);
    Q_PROPERTY(QString description READ description WRITE setDescription);

protected:
    qint32   m_id;
    QString  m_name;
    State    m_state;
    qint32   m_executorId;
    QDate    m_deadline;
    qint32   m_parentId;
    QString  m_description;
    QObject* m_executor;
};

QDebug operator<<(QDebug d, const Task* task);
