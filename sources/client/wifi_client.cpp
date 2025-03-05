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
{}

//-----------------------------------------------------------------------------
QStringListModel *WifiHttpClient::getNetworkModel()
{
    return &m_model;
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
    auto jsonDoc = QJsonDocument::fromJson(i_data);
    Q_ASSERT(jsonDoc.isArray());

    QStringList networkNames;
    for (auto &&val : jsonDoc.array())
        networkNames << val.toString();

    // update the model
    m_model.setStringList(networkNames);
}

//-----------------------------------------------------------------------------
void WifiHttpClient::_handlePasswordValidation(const QByteArray &i_data)
{
    Q_UNUSED(i_data);
}
