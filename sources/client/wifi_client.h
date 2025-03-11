#pragma once

#include "wifi_model.h"

#include <communication/client.h>

class QNetworkReply;

class WifiHttpClient : public Communication::HttpClient {
  Q_OBJECT
  Q_PROPERTY(WifiNetworkModel *wifiModel READ wifiModel NOTIFY wifiModelChanged)

public:
  WifiHttpClient(const QString &i_hostName, quint16 i_port,
                 QObject *ip_parent = nullptr);

  WifiNetworkModel *wifiModel() const;

  Q_INVOKABLE void requestNetworkList();
  Q_INVOKABLE void requestValidatePassword(const QString &i_networkName,
                                           const QString &i_password);

signals:
  void wifiModelChanged();
  void passwordValidatedWithResult(bool, const QString &);

private:
  std::optional<std::pair<int, QJsonObject>>
  _handleReply(QNetworkReply *ip_reply);

  void _handleNetworkList(QNetworkReply *ip_reply);
  void _handlePasswordValidation(QNetworkReply *ip_reply);

  void _processNetworkList(int i_statusCode, const QJsonObject &i_jsonObj);
  void _processPasswordValidation(int i_statusCode,
                                  const QJsonObject &i_jsonObj);

private:
  WifiNetworkModel *mp_model;
};
