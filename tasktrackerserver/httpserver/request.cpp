
#include "request.h"

Request::Request(const QString &request)
{
    if (request.isEmpty())
        throw std::runtime_error("the request is empty");

    // Разделяем заголовок и тело запроса, должно быть минимум
    // одна пустая строка, иначе непредвиденный конец запроса
    const QStringList requestParts = request.split("\r\n\r\n");
    if (requestParts.size() < 2)
        throw std::runtime_error("unexpected end of the request");

    m_data = requestParts.last();

    // Получаем стартовую строку и заголовки
    QStringList headers = requestParts.first().split("\r\n");
    const QString startLine = headers.first();
    headers.removeFirst();

    startLineParse(startLine);
    headersParse(headers);
}

Request::Method Request::method() const {
    return m_method;
}

const QString& Request::methodString() const {
    return m_methodString;
}

const QString& Request::uri() const {
    return m_uri;
}

const QString& Request::httpVersion() const {
    return m_httpVersion;
}

const QHash<QString, QString>& Request::headers() const {
    return m_headers;
}

const QString &Request::data() const {
    return m_data;
}

void Request::startLineParse(const QString& startLine)
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

void Request::headersParse(const QStringList& headers)
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
}

const QString Request::uriToEndPoint(const QString& uri)
{
    // Разделяем на элементы все, что между символами /
    QStringList tree = uri.split('/');

    QString endPoint = "/";

    for (auto& item : tree)
    {
        if (item.isEmpty())
            continue;

        bool result = false;

        // Если элеент приводится к числу, значит это шаблон идентификатора
        (void) item.toInt(&result);
        if (result)
            item = "{id}";

        endPoint += (item);

        if (item != tree.last())
            endPoint += '/';
    }

    //endPoint.replace(QRegExp("/[0-9]"), "/{id}");

    return endPoint;
}
