#include "wifi_server.h"

#include <communication/settings.h>
#include <communication/wifi_api.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

using namespace Communication;

//-----------------------------------------------------------------------------
WifiHttpServer::WifiHttpServer(QJsonObject &&i_settings)
    : HttpServer()
    , m_model(std::move(i_settings))
{
    route(WIFI_NETWORKS, [this](QTcpSocket *ip_socket, const QString &i_requestStr) {
        _handleNetworkListRequest(ip_socket, i_requestStr);
    });

    route(VALIDATE_WIFI_PWD, [this](QTcpSocket *ip_socket, const QString &i_requestStr) {
        _handleAuthenticationRequest(ip_socket, i_requestStr);
    });
}

//-----------------------------------------------------------------------------
void WifiHttpServer::_handleNetworkListRequest(QTcpSocket *ip_socket, const QString &i_requestStr)
{
    Q_UNUSED(i_requestStr);
    QJsonArray networkList({QJsonObject{{"id", "wifi_1"}}, QJsonObject{{"id", "wifi_2"}}});

    QJsonObject responseObj{{"wifi_networks", networkList}};
    QByteArray responseData = QJsonDocument(responseObj).toJson(QJsonDocument::Indented);

    sendResponse(ip_socket, Status::_OK, responseData);
}

//-----------------------------------------------------------------------------
void WifiHttpServer::_handleAuthenticationRequest(QTcpSocket *ip_socket, const QString &i_requestStr)
{
    // request example: {"id": "wifi_1", "auth": "!QAZxsw2#EDCvfr4"}

    QByteArray requestData = i_requestStr.toUtf8();
    auto jsonDoc = QJsonDocument::fromJson(requestData);
    auto jsonObj = jsonDoc.object();

    QJsonObject responseObj;
    QString status;

    if (!jsonObj.contains("id") || jsonObj["id"].toString().isEmpty()) {
        responseObj["error"] = "Missing or invalid 'id' parameter";
        status = Status::_BAD_REQUEST;
    } else if (!jsonObj.contains("auth") || jsonObj["auth"].toString().isEmpty()) {
        responseObj["error"] = "Missing or invalid 'auth' parameter";
        status = Status::_BAD_REQUEST;
    } else {
        auto networkId = jsonObj["id"].toString();
        if (m_model.contains(networkId)) {
            bool valid = (m_model[networkId] == jsonObj["auth"].toString());
            responseObj["password_valid"] = valid;
            status = valid ? Status::_OK : Status::_UNAUTHORIZED;
        } else {
            responseObj["error"] = "Network ID not found";
            status = Status::_NOT_FOUND; // 404
        }
    }

    QByteArray responseData = QJsonDocument(responseObj).toJson(QJsonDocument::Indented);
    sendResponse(ip_socket, status, responseData);
}
