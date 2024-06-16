
#include "httpsockethandler.h"

// =============================================================================
HttpSocketHandler::HttpSocketHandler(int handle, const RoutingMap& routingMap) :
    m_handle(handle),
    m_routingMap(routingMap)
{

}

// =============================================================================
void HttpSocketHandler::run()
{
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(m_handle);

    // Принимаем данные, учитывая, что они могут быть разбиты по частям
    QByteArray recvBuf;
    unsigned int countPackets = 0;
    while (socket->waitForReadyRead(100))
    {
        recvBuf.append(socket->readAll());
        ++(countPackets);
    }

    qDebug() << "---------------------";
    qDebug() << "TCP-packets received:" << countPackets;

    try
    {
        // Создаем объект-запрос из принятого буфера
        HttpRequest request(recvBuf);
        request.debug();

        // Обработка запроса
        auto response = requestProcess(request);

        // Отправляем ответ, закрываем сокет
        socket->write(response.toByteArray());
        socket->waitForBytesWritten();
    }
    catch (const std::runtime_error& ex) {
        qDebug() << "Error:" << ex.what();
        qDebug() << recvBuf;
    }

    socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();

    qDebug() << "Socket closed!";
}

// =============================================================================
HttpResponse HttpSocketHandler::requestProcess(const HttpRequest& request)
{
    // Пробегаемся по всем ендпоинтам, ищем подходящий
    for (auto it = m_routingMap.cbegin(); it != m_routingMap.cend(); ++it)
    {
        // Если метод не совпадает, переходим к следующему
        auto& routingMethod = it.key().first;
        if (routingMethod != request.method())
            continue;

        // Если ендпоинт и uri полностью совпадают,
        // то вызываем сразу обработчик
        auto& routingEndPoint = it.key().second;
        if (routingEndPoint == request.uri())
            return it.value()(request.content(), RoutingArgs());

        // Если вложенность uri не совпадает, то переходим к следующему
        if (routingEndPoint.count('/') != request.uri().count('/'))
            continue;

        // Разбиваем на лексемы ендпоинт и uri
        const QStringList endPointTokens = routingEndPoint.split('/');
        const QStringList uriTokens = request.uri().split('/');

        // Извлекаем аргументы из uri по шаблону ендпоинта
        RoutingArgs args;
        for (int i = 0; i < uriTokens.size(); ++i)
        {
            // Если лексемы совпадают, то переходим к следующей лексеме
            if (endPointTokens[i] == uriTokens[i])
                continue;

            // Если лексема заключена в скобки {}, то это аргумент
            const QString token = endPointTokens.at(i);
            if (token.front() == '{' && token.back() == '}') {
                args.insert(token, QVariant(uriTokens.at(i)));
            } else {
                // Лексемы не совпадают, поэтому этот ендпоинт не подходит,
                // переходим к следующему
                args.clear();
                break;
            }
        }

        // Если аргументов нет, значит ендпоинт не подошел и
        // переходим к следующему
        if (args.isEmpty())
            continue;

        qDebug() << "EndPoint:" << routingEndPoint;

        // Если нашли нужный обработчик, выполняем его
        return it.value()(request.content(), args);
    }

    return HttpResponse(HttpResponse::Status::NotFound);
}

// =============================================================================
