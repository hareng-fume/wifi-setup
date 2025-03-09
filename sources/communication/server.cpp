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

//-----------------------------------------------------------------------------
static bool requestIsComplete(const QByteArray &data) {
    int headerEnd = data.indexOf("\r\n\r\n");
    if (headerEnd == -1)
        return false; // headers not fully received

    int contentLength = 0;
    QRegExp regex("Content-Length: (\\d+)");
    if (regex.indexIn(data) != -1)
        contentLength = regex.cap(1).toInt();

    int totalExpectedSize = headerEnd + 4 + contentLength; // 4 = \r\n\r\n
    return data.size() >= totalExpectedSize;
}

} // namespace _Details

namespace Communication {

//-----------------------------------------------------------------------------
HttpServer::HttpServer(QObject *ip_parent /*= nullptr*/)
    : QTcpServer(ip_parent)
{
    connect(this, &QTcpServer::newConnection, this, &HttpServer::onNewConnection);
}

//-----------------------------------------------------------------------------
void HttpServer::incomingConnection(qintptr i_socketDescriptor)
{
    auto* p_socket = new QTcpSocket(this);
    p_socket->setSocketDescriptor(i_socketDescriptor);
    addPendingConnection(p_socket);
}

//-----------------------------------------------------------------------------
void HttpServer::onNewConnection()
{
    auto *p_socket = nextPendingConnection();
    if (!p_socket) {
        qDebug() << "Warning: " << "no next pending connection";
        return;
    }

    qDebug() << "New connection - is emitted once for every client that connects";

    // use p_socket to communicate with the client
    connect(p_socket, &QTcpSocket::readyRead, p_socket, [this, p_socket]() {
        processRequest(p_socket);
    });
    connect(p_socket, &QTcpSocket::disconnected, this, &HttpServer::onDisconnected);
}

//-----------------------------------------------------------------------------
void HttpServer::onDisconnected() {
    auto* p_socket = qobject_cast<QTcpSocket*>(sender());
    if (p_socket){
        p_socket->close();
        p_socket->deleteLater();
    }
}

//-----------------------------------------------------------------------------
void HttpServer::processRequest(QTcpSocket *ip_socket)
{
    m_buffer[ip_socket] += ip_socket->readAll();

    if (_Details::requestIsComplete(m_buffer[ip_socket])) {
        auto requestStr = QString::fromUtf8(m_buffer[ip_socket]);
        qDebug() << "request is complete: " << requestStr;
        auto path = _Details::extractPath(requestStr);
        if (m_routes.contains(path)) {
            m_routes[path](ip_socket, requestStr);
        } else {
            sendResponse(ip_socket, "404 Not Found", "Page not found");
        }
        m_buffer.remove(ip_socket);
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
    ip_socket->waitForBytesWritten(3000);
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
        qDebug() << "Server failed to start! " << errorString();
    else
        qDebug() << "Server listening on " << i_address << ", port " << i_port << "...";
}

} // namespace Communication
