
/*
 * Класс, который открывает/закрывает БД
 */

#pragma once

#include <QSqlDatabase>

#include "dbexception.h"

class DBOpener
{
public:
    DBOpener(QSqlDatabase* db) : m_db(db)
    {
        if (!m_db->open()) {
            throw DBException(m_db->lastError(), __FILE__, __LINE__);
        }
    }

    ~DBOpener()
    {
        if (m_db->isOpen())
            m_db->close();
    }

private:
    QSqlDatabase* m_db;
};
