
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

    // Принимаем данные, учитывая, что могут быть отправлены по частям
    QByteArray buf;
    HttpRequest request;
    while (!request.isValid() && buf.size() < 10000)
    {
        socket->waitForReadyRead();

        buf.append(socket->readAll());
        //qDebug() << buf;

        // Пробуем распарсить запрос
        try {
            request.parse(buf);
        } catch (const std::runtime_error& ex) {
            qDebug() << ex.what();
            buf.clear();
            continue;
        }

        // Если запрос валидный, то переходим для его обработки
        if (request.isValid())
            break;
    }

    qDebug() << request;

    try
    {
        // Обработка запроса
        auto response = requestProcess(request);

        // Отправляем ответ, закрываем сокет
        socket->write(response.toByteArray());
        socket->waitForBytesWritten();
    }
    catch (const std::runtime_error& ex) {
        qDebug() << ex.what();
    }

    socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
    qDebug() << "complete! Socket closed!";
}

// =============================================================================
HttpResponse HttpSocketHandler::requestProcess(const HttpRequest& request)
{
    auto endPoint = HttpRequest::uriToEndPoint(request.uri());
    qDebug() << "EndPoint:" << endPoint;

    // Ищем обработчик эндпоинта
    auto it = m_routingMap.find(QPair(request.method(), endPoint));

    // Если не нашли зарегистрированного эндпоинта,
    // то сообщаем что ресурс не найден
    if (it == m_routingMap.end()) {
        return HttpResponse(HttpResponse::Status::NotFound);
    }

    // Выполняем обработчик запроса
    return it.value()(request);
}

// =============================================================================
