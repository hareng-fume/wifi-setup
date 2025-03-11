#include "wifi_client.h"

#include <communication/wifi_api.h>

#include <QByteArray>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QNetworkReply>

using namespace Communication;

namespace _Details {

//-----------------------------------------------------------------------------
static void _logError(int i_statusCode, const QString &i_errMessage) {
  auto message = QString("%1 %2").arg(QString::number(i_statusCode),
                                      Status::getMessageFrom(i_statusCode));

  if (!i_errMessage.isEmpty())
    message += " - " + i_errMessage;

  qCritical() << "ERROR: " << message;
}

//-----------------------------------------------------------------------------
static void _logInfo(int i_statusCode, const QString &i_message) {
  auto message = QString("%1, %2").arg(QString::number(i_statusCode),
                                       Status::getMessageFrom(i_statusCode));

  if (!i_message.isEmpty())
    message += " - " + i_message;

  qInfo() << "INFO: " << message;
}

//-----------------------------------------------------------------------------
static void _log(int i_statusCode, const QJsonObject &i_jsonObj) {
  if (i_statusCode == 200)
    _logInfo(i_statusCode, i_jsonObj.value("message").toString());
  else
    _logError(i_statusCode, i_jsonObj.value("error").toString());
}

} // namespace _Details

//-----------------------------------------------------------------------------
WifiHttpClient::WifiHttpClient(const QString &i_hostName, quint16 i_port,
                               QObject *ip_parent /*= nullptr*/)
    : HttpClient(i_hostName, i_port, ip_parent),
      mp_model(new WifiNetworkModel(this)) {}

//-----------------------------------------------------------------------------
WifiNetworkModel *WifiHttpClient::wifiModel() const { return mp_model; }

//-----------------------------------------------------------------------------
void WifiHttpClient::requestNetworkList() {
  sendRequest(
      HttpMethod::GET, WIFI_NETWORKS, {},
      [this](QNetworkReply *ip_reply) { _handleNetworkList(ip_reply); });
}

//-----------------------------------------------------------------------------
void WifiHttpClient::requestValidatePassword(const QString &i_networkName,
                                             const QString &i_password) {
  QJsonDocument doc({{"id", i_networkName}, {"auth", i_password}});
  auto data = doc.toJson(QJsonDocument::Compact);

  sendRequest(
      HttpMethod::POST, VALIDATE_WIFI_PWD, data,
      [this](QNetworkReply *ip_reply) { _handlePasswordValidation(ip_reply); });
}

//-----------------------------------------------------------------------------
std::optional<std::pair<int, QJsonObject>>
WifiHttpClient::_handleReply(QNetworkReply *ip_reply) {

  ip_reply->deleteLater();

  auto replyData = ip_reply->readAll();
  auto statusCode =
      ip_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  if (ip_reply->error() != QNetworkReply::NoError)
    qCritical() << "ERROR: " << statusCode << " " << ip_reply->errorString();

  QJsonParseError jsonError;
  auto jsonDocument = QJsonDocument::fromJson(replyData, &jsonError);

  return std::make_pair(statusCode, jsonDocument.object());
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handleNetworkList(QNetworkReply *ip_reply) {
  auto result = _handleReply(ip_reply);
  _processNetworkList(result.value().first, result.value().second);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handlePasswordValidation(QNetworkReply *ip_reply) {
  auto result = _handleReply(ip_reply);
  if (result.has_value())
    _processPasswordValidation(result.value().first, result.value().second);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_processNetworkList(int i_statusCode,
                                         const QJsonObject &i_jsonObj) {

  if (i_statusCode != 200)
    return;

  if (!i_jsonObj.contains("wifi_ids") || !i_jsonObj["wifi_ids"].isArray()) {
    qWarning() << "WARNING: 'wifi_ids' missing or not an array in response";
    return;
  }

  WifiNetworkModel::ItemList networkList;
  for (auto &&val : i_jsonObj["wifi_ids"].toArray())
    networkList << WifiNetworkModel::Item(val.toString(),
                                          WifiNetwork::Disconnected);

  mp_model->resetItems(networkList);
  emit wifiModelChanged();

  _Details::_log(i_statusCode, i_jsonObj);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_processPasswordValidation(int i_statusCode,
                                                const QJsonObject &i_jsonObj) {

  const auto result =
      i_statusCode == 200
          ? std::make_pair(true, i_jsonObj.value("message").toString())
          : std::make_pair(false, i_jsonObj.value("error").toString());

  auto connectionStatus =
      result.first ? WifiNetwork::Connected : WifiNetwork::FailedToConnect;
  mp_model->setConnectionStatus(i_jsonObj.value("id").toString(),
                                connectionStatus);
  _Details::_log(i_statusCode, i_jsonObj);
}
