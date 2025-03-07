#include "wifi_client.h"
#include <wifi_api.h>

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>

using namespace Communication;

namespace _Details {

//-----------------------------------------------------------------------------
static void _logError(int i_statusCode, const QString &i_errMessage)
{
    static const QHash<int, QString> _g_error_code_to_msg = {{400, "Bad Request"},
                                                             {401, "Unauthorized"},
                                                             {404, "Not Found"},
                                                             {500, "Internal Server Error"}};

    if (_g_error_code_to_msg.contains(i_statusCode)) {
        qDebug() << i_statusCode << " " << _g_error_code_to_msg[i_statusCode];
        if (!i_errMessage.isEmpty())
            qDebug() << ": " << i_errMessage;
    } else {
        qDebug() << "Error: Http Status Code " << i_statusCode;
        if (!i_errMessage.isEmpty())
            qDebug() << ", Message: " << i_errMessage;
    }
}
} // namespace _Details

//-----------------------------------------------------------------------------
WifiHttpClient::WifiHttpClient(const QString &i_hostName,
                               quint16 i_port,
                               QObject *ip_parent /*= nullptr*/)
    : Communication::HttpClient(i_hostName, i_port, ip_parent)
    , mp_model(new QStringListModel(this))
{}

//-----------------------------------------------------------------------------
QStringListModel *WifiHttpClient::networkModel() const
{
    return mp_model;
}

//-----------------------------------------------------------------------------
void WifiHttpClient::requestNetworkList()
{
    sendGetRequest(WIFI_NETWORKS, [this](QNetworkReply *ip_reply) { _handleNetworkList(ip_reply); });
}

//-----------------------------------------------------------------------------
void WifiHttpClient::connectToNetwork(const QString &i_name, const QString &i_password)
{
    // e.g. jsonObj = {"id": "wifi_1", "auth": "!QAZxsw2#EDCvfr4"}

    QJsonObject jsonObj({{"id", i_name}, {"auth", i_password}});
    sendPostRequest(VALIDATE_WIFI_PWD, std::move(jsonObj), [this](QNetworkReply *ip_reply) {
        _handlePasswordValidation(ip_reply);
    });
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handleNetworkList(QNetworkReply *ip_reply)
{
    QByteArray replyData = ip_reply->readAll();
    auto jsonDoc = QJsonDocument::fromJson(replyData);
    auto jsonObj = jsonDoc.object();

    int statusCode = ip_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 200) {
        if (jsonObj.contains("wifi_ids")) {
            QStringList networkNames;
            for (auto &&val : jsonObj["wifi_ids"].toArray())
                networkNames << val.toString();

            // update the model
            mp_model->setStringList(networkNames);
            emit networkListUpdated();
        } else {
            qDebug() << "WARNING: Can't retrieve network list";
        }
    } else {
        auto errMsg = jsonObj.contains("error") ? jsonObj["error"].toString() : "";
        _Details::_logError(statusCode, errMsg);
    }
    ip_reply->deleteLater();
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handlePasswordValidation(QNetworkReply *ip_reply)
{
    QByteArray responseData = ip_reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();

    int statusCode = ip_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 200) {
        auto msg = jsonObj.contains("message") ? jsonObj["message"].toString() : "";
        emit passwordValidatedWithResult(true, msg);
    } else {
        auto errMsg = jsonObj.contains("error") ? jsonObj["error"].toString() : "";
        _Details::_logError(statusCode, errMsg);
        emit passwordValidatedWithResult(false, errMsg);
    }
    ip_reply->deleteLater();
}
