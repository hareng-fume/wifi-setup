#include "server.h"

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTcpSocket>

namespace _Details {

//-----------------------------------------------------------------------------
static QString extractPath(const QString &i_request)
{
    auto lines = i_request.split("\r\n");
    if (lines.isEmpty())
        return "/";
    auto requestLine = lines[0].split(" ");
    return requestLine.size() > 1 ? requestLine[1] : "/";
}

} // namespace _Details

namespace Communication {

//-----------------------------------------------------------------------------
HttpServer::HttpServer(QObject *ip_parent /*= nullptr*/)
    : QTcpServer(ip_parent)
{
    connect(this, &QTcpServer::newConnection, this, &HttpServer::handleNewConnection);
}

//-----------------------------------------------------------------------------
void HttpServer::incomingConnection(qintptr i_socketDescriptor)
{
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(i_socketDescriptor);
    connect(clientSocket, &QTcpSocket::readyRead, this, [clientSocket]() {
        QByteArray requestData = clientSocket->readAll();
        qDebug() << "Received request:\n" << requestData;

        // Prepare JSON response
        QJsonArray wifiArray;
        wifiArray.append(QJsonObject{{"id", "wifi_1"}, {"auth", "!QAZxsw2#EDCvfr4"}});
        wifiArray.append(QJsonObject{{"id", "wifi_2"}, {"auth", "vgy7*UHBnji9)OKLM"}});

        QJsonObject responseObj;
        responseObj["wifi_params"] = wifiArray;
        QByteArray responseData = QJsonDocument(responseObj).toJson(QJsonDocument::Indented);

        // Prepare HTTP response headers
        QByteArray httpResponse = "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: application/json\r\n"
                                  "Content-Length: "
                                  + QByteArray::number(responseData.size())
                                  + "\r\n"
                                    "Connection: close\r\n\r\n"
                                  + responseData;

        clientSocket->write(httpResponse);
        clientSocket->flush();
        clientSocket->disconnectFromHost();
    });
}

//-----------------------------------------------------------------------------
void HttpServer::handleNewConnection()
{
    auto *p_socket = nextPendingConnection();
    connect(p_socket, &QTcpSocket::readyRead, p_socket, [this, p_socket]() {
        processRequest(p_socket);
    });
    connect(p_socket, &QTcpSocket::disconnected, p_socket, &QTcpSocket::deleteLater);
}

//-----------------------------------------------------------------------------
void HttpServer::processRequest(QTcpSocket *ip_socket)
{
    auto requestData = ip_socket->readAll();
    auto requestStr = QString::fromUtf8(requestData);

    qDebug() << "Received request:\n" << requestStr;

    auto path = _Details::extractPath(requestStr);
    if (m_routes.contains(path)) {
        m_routes[path](ip_socket, requestStr);
    } else {
        sendResponse(ip_socket, "404 Not Found", "Page not found");
    }
}

//-----------------------------------------------------------------------------
void HttpServer::sendResponse(QTcpSocket *ip_socket,
                              const QString &i_status,
                              const QByteArray &i_responseData)
{
    // prepare HTTP response headers
    QByteArray httpResponse = "HTTP/1.1 " + i_status.toUtf8()
                              + "\r\n"
                                "Content-Type: application/json\r\n"
                                "Content-Length: "
                              + QByteArray::number(i_responseData.size())
                              + "\r\n"
                                "Connection: close\r\n\r\n"
                              + i_responseData;
    ip_socket->write(httpResponse);
    ip_socket->flush();
    ip_socket->disconnectFromHost();
}

//-----------------------------------------------------------------------------
void HttpServer::route(const QString &i_endpoint,
                       std::function<void(QTcpSocket *, const QString &)> i_callback)
{
    m_routes[i_endpoint] = i_callback;
}

//-----------------------------------------------------------------------------
void HttpServer::start(const QString &i_address, quint16 i_port /*= 8080*/)
{
    QHostAddress hostAddress;
    if (!hostAddress.setAddress(i_address)) {
        qCritical() << "Invalid IP address provided: " << i_address;
        return;
    }

    if (!listen(hostAddress, i_port))
        qDebug() << "Server failed to start!";
    else
        qDebug() << "Server listening on " << i_address << ", port " << i_port << "...";
}

} // namespace Communication
