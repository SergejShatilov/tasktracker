
#include "settings.h"

#include <QLocale>

// =============================================================================
Settings::Settings(const QString& fileName, QObject* parent) :
    QObject(parent),
    m_settings(new QSettings(fileName, QSettings::IniFormat, this))
{
    m_settings->beginGroup("interface");
    m_language = m_settings->value("language", "sys").toString();
    m_settings->endGroup();

    m_settings->beginGroup("server");
    m_hostName = m_settings->value("host", "127.0.0.1").toString();
    m_port = m_settings->value("port", 80).toInt();
    m_settings->endGroup();

    if (m_language == "sys")
        m_language = QLocale::system().name();
}

// =============================================================================
void Settings::commit()
{
    m_settings->beginGroup("interface");
    m_settings->setValue("language", m_language);
    m_settings->endGroup();

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
void Settings::setLanguage(const QString& language) {
    m_language = language;
}

const QString& Settings::language() const {
    return m_language;
}

// =============================================================================
