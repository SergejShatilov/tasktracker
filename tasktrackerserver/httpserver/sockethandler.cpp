
#include "sockethandler.h"
#include "request.h"
#include "response.h"

SocketHandler::SocketHandler(int handle, const RoutingMap& routingMap) :
    m_handle(handle),
    m_routingMap(routingMap)
{

}

void SocketHandler::run()
{
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(m_handle);

    // Принимаем запрос
    socket->waitForReadyRead();

    const QString requestString { QString(socket->readAll()) };
    Request request(requestString);

    qDebug() << "REQUEST:" << requestString;
    qDebug() << "\tSTART LINE:";
    qDebug() << "\t\tMETHOD:" << request.methodString();
    qDebug() << "\t\tURI:" << request.uri();
    qDebug() << "\t\tHTTP VERSION:" << request.httpVersion();
    qDebug() << "\tHEADERS:";
    const auto& headers = request.headers();
    for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
        qDebug() << "\t\t" << it.key() << ":" << it.value();
    }
    qDebug() << "\tBODY:" << request.data();

    // Обработка запроса
    auto response = requestProcess(request);

    // Отправляем ответ, закрываем сокет
    socket->write(response.toByteArray());
    socket->waitForBytesWritten();

    socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
}

Response SocketHandler::requestProcess(const Request& request)
{
    auto endPoint = Request::uriToEndPoint(request.uri());
    qDebug() << "ENDPOINT:" << endPoint;

    // Ищем обработчик эндпоинта
    auto it = m_routingMap.find(QPair(request.method(), endPoint));

    // Если не нашли зарегистрированного эндпоинта,
    // то сообщаем что ресурс не найден
    if (it == m_routingMap.end()) {
        return Response(Response::Status::NotFound);
    }

    // Выполняем обработчик запроса
    return it.value()(request);
}
