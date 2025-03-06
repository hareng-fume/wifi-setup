#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

#include <communication/server.h>

#include <QJsonObject>

class WifiHttpServer : public Communication::HttpServer
{
public:
    explicit WifiHttpServer(QJsonObject &&i_settings);

private:
    void _handleNetworkListRequest(QTcpSocket *ip_socket, const QString &i_requestStr);
    void _handleAuthenticationRequest(QTcpSocket *ip_socket, const QString &i_requestStr);

private:
    QJsonObject m_model;
};

#endif // WIFI_SERVER_H
