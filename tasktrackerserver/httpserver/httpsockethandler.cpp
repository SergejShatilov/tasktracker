
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

    // Принимаем запрос
    socket->waitForReadyRead();

    try
    {
        // Парсим запрос
        auto buf = socket->readAll();
        qDebug() << buf;
        HttpRequest request(buf);
        qDebug() << request;

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
