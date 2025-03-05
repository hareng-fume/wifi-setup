#include "server.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

namespace Communication {

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

} // namespace Communication
