#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <communication/server.h>

#include <QMap>
#include <QString>

class WifiHttpServer : public Communication::HttpServer
{
public:
    explicit WifiHttpServer(QObject *ip_parent = nullptr);
    void loadWifiCredentials(QJsonObject &&i_settings);

private:
    void _handleNetworkListRequest(QTcpSocket *ip_socket, const QString &i_requestStr);
    void _handleAuthenticationRequest(QTcpSocket *ip_socket, const QString &i_requestStr);

private:
    QMap<QString, QString> m_wifiCredentials;
};

#endif // WIFI_SERVER_H
