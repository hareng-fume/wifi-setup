#include "wifi_server.h"

#include <communication/wifi_api.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Communication;

namespace _Details {

//-----------------------------------------------------------------------------
static QJsonObject _extractJSONPayload(const QString& i_requestStr)
{
    // HTTP headers and body are separated by two consecutive newlines (\r\n\r\n)
    int jsonStart = i_requestStr.indexOf("\r\n\r\n");

    if (jsonStart != -1) {
        QString jsonStr = i_requestStr.mid(jsonStart + 4).trimmed();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());

        if (!jsonDoc.isNull() && jsonDoc.isObject())
            return jsonDoc.object();
    }
    return {};
}

} // namespace _Details

//-----------------------------------------------------------------------------
WifiHttpServer::WifiHttpServer(QObject* ip_parent /*= nullptr*/)
    : HttpServer(ip_parent)
{
    route(WIFI_NETWORKS, [this](QTcpSocket* ip_socket, const QString& i_requestStr) {
        _handleNetworkListRequest(ip_socket, i_requestStr);
    });

    route(VALIDATE_WIFI_PWD, [this](QTcpSocket* ip_socket, const QString& i_requestStr) {
        _handleAuthenticationRequest(ip_socket, i_requestStr);
    });
}

//-----------------------------------------------------------------------------
void WifiHttpServer::loadWifiCredentials(const QJsonObject& i_settings)
{
    m_credentials.clear();

    if (!i_settings.contains("wifi_params") || !i_settings["wifi_params"].isArray()) {
        qCritical() << "ERROR: Invalid JSON format: missing 'wifi_params'";
        return;
    }

    for (auto &&value : i_settings["wifi_params"].toArray()) {
        auto jsonObj = value.toObject();
        if (jsonObj.contains("id") && jsonObj.contains("auth")) {
            auto id = jsonObj["id"].toString();
            auto auth = jsonObj["auth"].toString();
            if (!id.isEmpty() && !auth.isEmpty())
                m_credentials[id] = auth;
        }
    }
    qDebug() << "Loaded WiFi credentials:" << m_credentials;
}

//-----------------------------------------------------------------------------
void WifiHttpServer::_handleNetworkListRequest(QTcpSocket* ip_socket, const QString& i_requestStr)
{
    Q_UNUSED(i_requestStr);

    QJsonObject responseObj;
    QString status;

    if (m_credentials.isEmpty()) {
        responseObj["error"] = "No available Wi-Fi networks found during scanning.";
        status = Status::BAD_REQUEST;
    } else {
        responseObj["wifi_ids"] = QJsonArray::fromStringList(m_credentials.keys());
        status = Status::OK;
    }

    auto responseData = QJsonDocument(responseObj).toJson(QJsonDocument::Compact);
    sendResponse(ip_socket, status, responseData);
}

//-----------------------------------------------------------------------------
void WifiHttpServer::_handleAuthenticationRequest(QTcpSocket* ip_socket,
                                                  const QString& i_requestStr)
{
    QJsonObject responseObj;
    QString status;

    auto requestJsonObj = _Details::_extractJSONPayload(i_requestStr);
    if (requestJsonObj.isEmpty()) {
        responseObj["error"] = "Invalid or impossible to extract JSON";
        status = Status::BAD_REQUEST;
    } else {
        // request example: {"id": "wifi_1", "auth": "!QAZxsw2#EDCvfr4"}

        if (!requestJsonObj.contains("id") || requestJsonObj["id"].toString().isEmpty()) {
            responseObj["error"] = "Missing or invalid 'id' parameter";
            status = Status::BAD_REQUEST;
        } else if (!requestJsonObj.contains("auth") || requestJsonObj["auth"].toString().isEmpty()) {
            responseObj["error"] = "Missing or invalid 'auth' parameter";
            status = Status::BAD_REQUEST;
        } else {
            auto networkId = requestJsonObj["id"].toString();
            auto networkPass = requestJsonObj["auth"].toString();

            responseObj["id"] = networkId;
            if (m_credentials.contains(networkId)) {
                if (m_credentials[networkId] == networkPass) {
                    responseObj["message"] = QString("Accepted connection to '%1'").arg(networkId);
                    status = Status::OK;
                } else {
                    responseObj["error"] = "Wrong password";
                    status = Status::UNAUTHORIZED;
                }
            } else {
                responseObj["error"] = QString("Network '%1' not found").arg(networkId);
                status = Status::NOT_FOUND;
            }
        }
    }

    auto responseData = QJsonDocument(responseObj).toJson(QJsonDocument::Compact);
    sendResponse(ip_socket, status, responseData);
}
