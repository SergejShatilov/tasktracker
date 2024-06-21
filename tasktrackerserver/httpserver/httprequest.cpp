
#include "httprequest.h"

#include <QDebug>

// =============================================================================
HttpRequest::HttpRequest(const QByteArray& data)
{
    if (data.isEmpty())
        throw std::runtime_error("Request is empty");

    const QString requestString(data);

    // Разделяем заголовок и тело запроса, должно быть минимум
    // одна пустая строка, иначе непредвиденный конец запроса
    const QStringList requestParts = requestString.split("\r\n\r\n");
    if (requestParts.size() < 2)
        throw std::runtime_error("Unexpected end of the request");

    // Получаем контент
    m_content = requestParts.last().toLocal8Bit();

    // Получаем стартовую строку и заголовки
    QStringList headers = requestParts.first().split("\r\n");
    const QString startLine = headers.first();
    headers.removeFirst();

    startLineParse(startLine);
    headersParse(headers);

    checkContent();
}

// =============================================================================
HttpRequest::Method HttpRequest::method() const {
    return m_method;
}

const QString& HttpRequest::methodString() const {
    return m_methodString;
}

const QString& HttpRequest::uri() const {
    return m_uri;
}

const QString& HttpRequest::httpVersion() const {
    return m_httpVersion;
}

const QHash<QString, QString>& HttpRequest::headers() const {
    return m_headers;
}

const QByteArray& HttpRequest::content() const {
    return m_content;
}

// =============================================================================
void HttpRequest::debug() const
{
    qDebug() << QString("Request(%1 %2 %3):")
         .arg(m_methodString)
         .arg(m_uri)
         .arg(m_httpVersion)
         .toStdString().c_str();

    for (auto it = m_headers.cbegin(); it != m_headers.cend(); ++it) {
        qDebug() << QString("    %1 : %2")
                    .arg(it.key())
                    .arg(it.value())
                    .toStdString().c_str();;
    }

    if (!m_content.isEmpty()) {
        qDebug() << m_content;
    }
}

// =============================================================================
void HttpRequest::startLineParse(const QString& startLine)
{
    // Разбираем стартовую строку, должна состоять из трех частей:
    // Метода, URI и версии HTTP
    const QStringList startLineParts = startLine.split(' ');
    if (startLineParts.size() != 3)
        throw std::runtime_error("Starting line is invalid");

    m_methodString = startLineParts.at(0);
    m_uri          = startLineParts.at(1);
    m_httpVersion  = startLineParts.at(2);

    // Преобразуем в enum class
    static QHash<QString, Method> const tableMethods = {
        {"GET",    Method::Get},
        {"POST",   Method::Post},
        {"PUT",    Method::Put},
        {"DELETE", Method::Delete}
    };

    auto it = tableMethods.find(m_methodString);
    if (it != tableMethods.end()) {
        m_method = it.value();
    } else {
        throw std::runtime_error("Method is invalid");
    }

    // Проверяем URI, должен быть не пустой и иметь вначале /
    if (m_uri.isEmpty() || (m_uri[0] != '/'))
        throw std::runtime_error("Uri is invalid");

    // Проверяем версию HTTP, поддерживаем пока что HTTP/1.1
    if (m_httpVersion != "HTTP/1.1")
        throw std::runtime_error("Unsupported http version");
}

// =============================================================================
void HttpRequest::headersParse(const QStringList& headers)
{
    for (const auto& header : headers)
    {
        // В заголовке должно быть хотябы одно двоеточие
        if (!header.contains(':'))
            throw std::runtime_error("Header is invalid");

        int index = header.indexOf(':');
        const QString& key = header.left(index);
        const QString& value = header.mid(index + 2);
        m_headers.insert(key, value);
    }
}

// =============================================================================
void HttpRequest::checkContent()
{
    // Для GET- и DELETE- запросов контент не проверяем
    if (m_method == Method::Get || m_method == Method::Delete)
        return;

    // Проверяем размер контента
    /*auto it = m_headers.find("Content-Length");
    if (it != m_headers.end()) {
        if (m_content.length() != it.value().toInt()) {
            throw std::runtime_error("Size of the content does not match");
        }
        return;
    }

    throw std::runtime_error("Header 'Content-Length' not found");*/
}

// =============================================================================
QString HttpRequest::uriToEndPoint(const QString& uri)
{
    // Разделяем на элементы все, что между символами /
    QStringList tree = uri.split('/');

    QString endPoint = "/";

    for (auto& item : tree)
    {
        if (item.isEmpty())
            continue;

        // Если есть разделительное тире,
        // то заменяем вторую часть на абстракцию
        if (item.contains('-'))
        {
            const QString key = item.split('-').first();
            item = QString("%1-{%1}").arg(key);
        }

        endPoint += (item);

        if (item != tree.last())
            endPoint += '/';
    }

    return endPoint;
}

// =============================================================================
QString HttpRequest::findValueInUri(const QString& uri, const QString& key)
{
    QStringList tree = uri.split('/');

    for (auto& item : tree)
    {
        if (item.isEmpty())
            continue;

        if (item.contains(QString("%1-").arg(key)))
        {
            const QString value = item.split('-').last();
            return value;
        }
    }

    return QString();
}

// =============================================================================
