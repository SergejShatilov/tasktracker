
#include "settings.h"

// =============================================================================
Settings::Settings(const QString& fileName, QObject* parent) :
    QObject(parent),
    m_settings(new QSettings(fileName, QSettings::IniFormat, this))
{
    m_settings->beginGroup("server");
    m_hostName = m_settings->value("host", "127.0.0.1").toString();
    m_port = m_settings->value("port", 80).toInt();
    m_settings->endGroup();
}

// =============================================================================
void Settings::commit()
{
    m_settings->beginGroup("server");
    m_settings->setValue("host", m_hostName);
    m_settings->setValue("port", m_port);
    m_settings->endGroup();
}

// =============================================================================
void Settings::setHostName(const QString& hostName) {
    m_hostName = hostName;
}

const QString& Settings::hostName() const {
    return m_hostName;
}

// =============================================================================
void Settings::setPort(int port) {
    m_port = port;
}

int Settings::port() const {
    return m_port;
}

// =============================================================================
