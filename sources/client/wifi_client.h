#pragma once

#include <communication/client.h>

#include <QStringListModel>

class QNetworkReply;

class WifiHttpClient : public Communication::HttpClient
{
    Q_OBJECT
    Q_PROPERTY(QStringListModel* networkModel READ networkModel NOTIFY networkListUpdated)

public:
    WifiHttpClient(const QString& i_hostName, quint16 i_port, QObject* ip_parent = nullptr);

    QStringListModel* networkModel() const;

    Q_INVOKABLE void requestNetworkList();
    Q_INVOKABLE void requestValidatePassword(const QString& i_networkName, const QString& i_password);

signals:
    void networkListUpdated();
    void passwordValidatedWithResult(bool, const QString&);

private:
    std::optional<std::pair<int, QJsonObject>> _handleReply(QNetworkReply* ip_reply);

    void _handleNetworkList(QNetworkReply* ip_reply);
    void _handlePasswordValidation(QNetworkReply* ip_reply);

    void _processNetworkList(int i_statusCode, const QJsonObject& i_jsonObj);
    void _processPasswordValidation(int i_statusCode, const QJsonObject& i_jsonObj);

private:
    QStringListModel *mp_model;
};
