#include "wifi_client.h"

#include <communication/wifi_api.h>

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>
#include <QHash>

using namespace Communication;

namespace _Details {

//-----------------------------------------------------------------------------
static const QHash<int, QString> _codeToMsg = {
    {200, "Ok"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {404, "Not Found"},
    {500, "Internal Server Error"}
};

//-----------------------------------------------------------------------------
static void _logError(int i_statusCode, const QString& i_errMessage) {
    auto errorMsg = QString("Status Code: %1, %2").arg(QString::number(i_statusCode),
                _codeToMsg.value(i_statusCode, "Unknown Error"));

    if (!i_errMessage.isEmpty())
        errorMsg += " - " + i_errMessage;

    if (i_statusCode >= 500)            // critical server errors
        qCritical() << errorMsg;
    else                                // warnings for client errors
        qWarning() << errorMsg;
}

//-----------------------------------------------------------------------------
static void _logInfo(int i_statusCode, const QString& i_message) {
    auto message = QString("Status Code: %1, %2").arg(QString::number(i_statusCode),
                _codeToMsg.value(i_statusCode, "Unknown Code"));
    if (!i_message.isEmpty())
        message += " - " + i_message;
    qInfo() << message;
}

//-----------------------------------------------------------------------------
static void _log(int i_statusCode, const QJsonObject& i_jsonObj) {
    if (i_statusCode == 200)
        _logInfo(i_statusCode, i_jsonObj.value("message").toString());
    else
        _logError(i_statusCode, i_jsonObj.value("error").toString());
}

} // namespace _Details

//-----------------------------------------------------------------------------
WifiHttpClient::WifiHttpClient(const QString& i_hostName,
                               quint16 i_port,
                               QObject* ip_parent /*= nullptr*/)
    : HttpClient(i_hostName, i_port, ip_parent)
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
    sendRequest(HttpMethod::GET, WIFI_NETWORKS, {}
        , [this](QNetworkReply* ip_reply) { _handleNetworkList(ip_reply); });
}

//-----------------------------------------------------------------------------
void WifiHttpClient::requestValidatePassword(const QString &i_networkName, const QString &i_password)
{
    QJsonDocument doc({{"id", i_networkName}, {"auth", i_password}});
    auto data = doc.toJson(QJsonDocument::Compact);

    sendRequest(HttpMethod::POST, VALIDATE_WIFI_PWD, data
        , [this](QNetworkReply *ip_reply) { _handlePasswordValidation(ip_reply); });
}

//-----------------------------------------------------------------------------
std::optional<std::pair<int, QJsonObject>> WifiHttpClient::_handleReply(QNetworkReply* ip_reply) {

    ip_reply->deleteLater();
    auto replyData = ip_reply->readAll();

    QJsonParseError jsonError;
    auto jsonDocument = QJsonDocument::fromJson(replyData, &jsonError);

    if (jsonDocument.isNull() || !jsonDocument.isObject()) {
        qCritical() << "ERROR: Invalid JSON response - " << jsonError.errorString();
        return {};
    }

    auto jsonObj = jsonDocument.object();
    auto statusCode = ip_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    return std::make_pair(statusCode, jsonObj);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handleNetworkList(QNetworkReply* ip_reply) {
    auto result = _handleReply(ip_reply);
    if (result.has_value())
       _processNetworkList(result.value().first, result.value().second);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handlePasswordValidation(QNetworkReply* ip_reply) {
    auto result = _handleReply(ip_reply);
    if (result.has_value())
        _processPasswordValidation(result.value().first, result.value().second);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_processNetworkList(int i_statusCode, const QJsonObject& i_jsonObj) {

    if (i_statusCode == 200) {
        if (!i_jsonObj.contains("wifi_ids") || !i_jsonObj["wifi_ids"].isArray()) {
            qWarning() << "WARNING: 'wifi_ids' missing or not an array in response";
            return;
        }

        QStringList networkNames;
        for (auto&& val : i_jsonObj["wifi_ids"].toArray())
            networkNames << val.toString();

        mp_model->setStringList(networkNames);
        emit networkListUpdated();
    }

    _Details::_log(i_statusCode, i_jsonObj);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_processPasswordValidation(int i_statusCode, const QJsonObject& i_jsonObj) {

    const auto result = i_statusCode == 200 ?
                std::make_pair(true, i_jsonObj.value("message").toString()):
                std::make_pair(false, i_jsonObj.value("error").toString());
    emit passwordValidatedWithResult(result.first, result.second);

    _Details::_log(i_statusCode, i_jsonObj);
}

