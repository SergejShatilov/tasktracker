
/*
 * Класс представляет собой хранилище имени пользователя и пароля
 */

#pragma once

#include <QString>

class DBAuthorization
{
public:
    explicit DBAuthorization(const QString& userName,
                             const QString& passWord) :
        m_userName(userName),
        m_passWord(passWord)
    {

    }

    const QString& userName() const {
        return m_userName;
    }

    const QString& passWord() const {
        return m_passWord;
    }

private:
    QString m_userName;
    QString m_passWord;
};
