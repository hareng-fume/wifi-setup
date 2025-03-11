#pragma once

#include <communication/server.h>

#include <QMap>
#include <QString>

class QTcpSocket;

class WifiHttpServer : public Communication::HttpServer {
public:
  explicit WifiHttpServer(QObject *ip_parent = nullptr);
  void loadWifiCredentials(const QJsonObject &i_settings);

private:
  void _handleNetworkListRequest(QTcpSocket *ip_socket,
                                 const QString &i_requestStr);
  void _handleAuthenticationRequest(QTcpSocket *ip_socket,
                                    const QString &i_requestStr);

private:
  QMap<QString, QString> m_credentials;
};
