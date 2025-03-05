#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <client.h>

#include <QStringListModel>

class WifiHttpClient : public Communication::HttpClient
{
public:
    WifiHttpClient(const QString &i_hostName, quint16 i_port, QObject *ip_parent = nullptr);

    QStringListModel *getNetworkModel();

    void requestNetworkList();
    void connectToNetwork(const QString &i_name, const QString &i_password);

private:
    void _handleNetworkList(const QByteArray &i_data);

private:
    QStringListModel m_model;
};

#endif // WIFI_CLIENT_H
