
#include "httprequest.h"

// =============================================================================
HttpRequest::HttpRequest(const QByteArray& request)
{
    const QString requestString(request);

    if (requestString.isEmpty())
        throw std::runtime_error("the request is empty");

    // Разделяем заголовок и тело запроса, должно быть минимум
    // одна пустая строка, иначе непредвиденный конец запроса
    const QStringList requestParts = requestString.split("\r\n\r\n");
    if (requestParts.size() < 2)
        throw std::runtime_error("unexpected end of the request");

    m_data = requestParts.last().toLocal8Bit();

    // Получаем стартовую строку и заголовки
    QStringList headers = requestParts.first().split("\r\n");
    const QString startLine = headers.first();
    headers.removeFirst();

    startLineParse(startLine);
    headersParse(headers);
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

// =============================================================================
const QString& HttpRequest::userName() const {
    return m_userName;
}

const QString& HttpRequest::password() const {
    return m_password;
}

// =============================================================================
QString HttpRequest::dbname() const {
    return findValueInUri(m_uri, "dbname");
}

qint32 HttpRequest::id() const {
    return findValueInUri(m_uri, "id").toInt();
}

const QByteArray &HttpRequest::data() const {
    return m_data;
}

// =============================================================================
void HttpRequest::startLineParse(const QString& startLine)
{
    // Разбираем стартовую строку, должна состоять из трех частей:
    // Метода, URI и версии HTTP
    const QStringList startLineParts = startLine.split(' ');
    if (startLineParts.size() != 3)
        throw std::runtime_error("starting line is invalid");

    m_methodString = startLineParts.at(0);
    m_uri          = startLineParts.at(1);
    m_httpVersion  = startLineParts.at(2);

    // Преобразуем в enum class
    static QHash<QString, Method> const tableMethods = {
        {"GET",    Method::Get},
        {"POST",   Method::Post},
        {"PUT",    Method::Put},
        {"DELETE", Method::Delete},
        {"PATCH",  Method::Patch}
    };

    auto it = tableMethods.find(m_methodString);
    if (it != tableMethods.end()) {
        m_method = it.value();
    } else {
        throw std::runtime_error("method is invalid");
    }

    // Проверяем URI, должен быть не пустой и иметь вначале /
    if (m_uri.isEmpty() || (m_uri[0] != '/'))
        throw std::runtime_error("uri is invalid");

    // Проверяем версию HTTP, поддерживаем пока что HTTP/1.1
    if (m_httpVersion != "HTTP/1.1")
        throw std::runtime_error("unsupported http version");
}

// =============================================================================
void HttpRequest::headersParse(const QStringList& headers)
{
    for (const auto& header : headers)
    {
        // В заголовке должно быть хотябы одно двоеточие
        if (header.count(':') < 1)
            throw std::runtime_error("header is invalid");

        int index = header.indexOf(':');
        const QString& key = header.left(index);
        const QString& value = header.mid(index + 2);
        m_headers.insert(key, value);
    }

    // Ищем информацию для авторизации пользователя
    auto it = m_headers.find("Authorization");
    if (it != m_headers.end())
    {
        QStringList authorization = it.value().split(',');
        m_userName = authorization.first();
        m_password = authorization.last().simplified();
    }
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
