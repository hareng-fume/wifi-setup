#include "wifi_client.h"
#include <wifi_api.h>

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

using namespace Communication;

//-----------------------------------------------------------------------------
WifiHttpClient::WifiHttpClient(const QString &i_hostName,
                               quint16 i_port,
                               QObject *ip_parent /*= nullptr*/)
    : Communication::HttpClient(i_hostName, i_port, ip_parent)
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
    sendGetRequest(WIFI_NETWORKS, [this](QByteArray i_response) { _handleNetworkList(i_response); });
}

//-----------------------------------------------------------------------------
void WifiHttpClient::connectToNetwork(const QString &i_name, const QString &i_password)
{
    // e.g. jsonObj = {"id": "wifi_1", "auth": "!QAZxsw2#EDCvfr4"}

    QJsonObject jsonObj({{"id", i_name}, {"auth", i_password}});
    sendPostRequest(VALIDATE_WIFI_PWD, std::move(jsonObj), [this](QByteArray i_response) {
        _handlePasswordValidation(i_response);
    });
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handleNetworkList(const QByteArray &i_data)
{
    qDebug() << "received raw data: " << i_data;
    auto jsonDoc = QJsonDocument::fromJson(i_data);

    if (jsonDoc.object().contains("error")) {
        qDebug() << "can't get networks from server";
        return;
    }

    if (jsonDoc.object().contains("wifi_networks")) {
        QStringList networkNames;
        for (auto &&val : jsonDoc.object()["wifi_networks"].toArray())
            networkNames << val.toString();

        qDebug() << "networks received: " << networkNames;

        // update the model
        mp_model->setStringList(networkNames);
        emit networkListUpdated();
    }
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handlePasswordValidation(const QByteArray &i_data)
{
    Q_UNUSED(i_data);
}
