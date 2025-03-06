#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <client.h>

#include <QStringListModel>

class WifiHttpClient : public Communication::HttpClient
{
    Q_OBJECT
    Q_PROPERTY(QStringListModel *networkModel READ networkModel NOTIFY networkListUpdated)

public:
    WifiHttpClient(const QString &i_hostName, quint16 i_port, QObject *ip_parent = nullptr);

    QStringListModel *networkModel() const;

    Q_INVOKABLE void requestNetworkList();
    void connectToNetwork(const QString &i_name, const QString &i_password);

signals:
    void networkListUpdated();

private:
    void _handleNetworkList(const QByteArray &i_data);
    void _handlePasswordValidation(const QByteArray &i_data);

private:
    QStringListModel *mp_model;
};

#endif // WIFI_CLIENT_H
