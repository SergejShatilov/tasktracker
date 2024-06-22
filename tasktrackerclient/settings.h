
#pragma once

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(const QString& fileName,
                      QObject* parent = nullptr);

    void commit();

    void setHostName(const QString& hostName);
    const QString& hostName() const;

    void setPort(int port);
    int port() const;

    void setLanguage(const QString& language);
    const QString& language() const;

private:
    QSettings*  m_settings;
    QString     m_hostName;
    int         m_port;
    QString     m_language;

};
