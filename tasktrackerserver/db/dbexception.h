
#pragma once

#include <QSqlError>
#include <QDebug>

class DBException
{
public:
    explicit DBException(const QSqlError& error,
                         const QString& fileName = "",
                         const int line = 0) :
        m_error(error),
        m_fileName(fileName),
        m_line(line)
    {

    }

    const QSqlError& error() const {
        return m_error;
    }

    const QString& fileName() const {
        return m_fileName;
    }

    int line() const {
        return m_line;
    }

private:
    QSqlError m_error;
    QString   m_fileName;
    int       m_line;
};

inline QDebug operator<<(QDebug d, const DBException& ex)
{
    QString errorType = "UnknownError";
    switch (ex.error().type())
    {
        case QSqlError::ErrorType::NoError:
            errorType = "NoError";
            break;
        case QSqlError::ErrorType::ConnectionError:
            errorType = "ConnectionError";
            break;
        case QSqlError::ErrorType::StatementError:
            errorType = "StatementError";
            break;
        case QSqlError::ErrorType::TransactionError:
            errorType = "TransactionError";
            break;
        default:
            break;
    }

    d << "Exception:" << errorType << "\r\n";
    d << "What:" << ex.error().text() << "\r\n";
    d << "File:" << ex.fileName() << ":" << ex.line() << "\r\n";

    return d;
}

