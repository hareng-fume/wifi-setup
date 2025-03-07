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
    Q_INVOKABLE void connectToNetwork(const QString &i_name, const QString &i_password);

signals:
    void networkListUpdated();
    void passwordValidatedWithResult(bool, const QString &);

private:
    void _handleNetworkList(QNetworkReply *ip_reply);
    void _handlePasswordValidation(QNetworkReply *ip_reply);

private:
    QStringListModel *mp_model;
};

#endif // WIFI_CLIENT_H
