
#pragma once

#include <QString>
#include <QJsonObject>
#include <QSharedPointer>
#include <QDebug>

class Employee : public QObject
{
    Q_OBJECT

public:
    explicit Employee(QObject* parent = nullptr);

    bool isValid() const;

    void setId(qint32 val);
    qint32 id() const;

    void setLastName(const QString& val);
    const QString& lastName() const;

    void setFirstName(const QString& val);
    const QString& firstName() const;

    QString fullName() const;

    void setPatronymic(const QString& val);
    const QString& patronymic() const;

    void setPosition(const QString& val);
    const QString& position() const;

    void setEmail(const QString& val);
    const QString& email() const;

    void setPhone(const QString& val);
    const QString& phone() const;

    void addTask(QObject* task);
    void removeTask(QObject* task);
    const QList<QObject*>& tasks() const;

    // ----------------------------------------
    QJsonObject toJsonObject() const;

    void fromJson(const QByteArray& data);
    QByteArray toJson() const;

    static Employee* createFromJsonObject(
        const QJsonObject& jObj,
        QObject* parent = nullptr
    );

    static Employee* createFromJson(
        const QByteArray& data,
            QObject* parent = nullptr
    );

    static QList<Employee*> createListFromJson(
        const QByteArray& data,
        QObject* parent = nullptr
    );

    static QByteArray listToJson(
        const QList<QSharedPointer<Employee>>& listEmployees
    );

public:
    Q_PROPERTY(qint32 id READ id WRITE setId);
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName);
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName);
    Q_PROPERTY(QString patronymic READ patronymic WRITE setPatronymic);
    Q_PROPERTY(QString position READ position WRITE setPosition);
    Q_PROPERTY(QString email READ email WRITE setEmail);
    Q_PROPERTY(QString phone READ phone WRITE setPhone);
    Q_PROPERTY(QString fullName READ fullName);

private:
    qint32  m_id;
    QString m_lastName;
    QString m_firstName;
    QString m_patronymic;
    QString m_position;
    QString m_email;
    QString m_phone;

    QList<QObject*> m_tasks;
};

QDebug operator<<(QDebug d, const Employee* employee);
