#include "client.h"

#include <QDebug>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QtGlobal>

namespace Communication {

//-----------------------------------------------------------------------------
HttpClient::HttpClient(const QString &i_hostName, quint16 i_port, QObject *ip_parent /*=nullptr*/)
    : QObject(ip_parent)
    , m_baseUrl(QString("http://%1:%2").arg(i_hostName).arg(i_port))
    , mp_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(mp_networkAccessManager,
            &QNetworkAccessManager::finished,
            this,
            &HttpClient::onResponseReceived);
}

//-----------------------------------------------------------------------------
void HttpClient::sendGetRequest(const QString &i_endpoint,
                                std::function<void(QByteArray)> i_callback)
{
    QNetworkRequest request(m_baseUrl + i_endpoint);
    auto *p_reply = mp_networkAccessManager->get(request);

    m_requestMap.insert(p_reply, i_callback);
}

//-----------------------------------------------------------------------------
void HttpClient::sendPostRequest(const QString &i_endpoint,
                                 const QJsonObject &i_data,
                                 std::function<void(QByteArray)> i_callback)
{
    QNetworkRequest request(m_baseUrl + i_endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(i_data);
    auto jsonData = doc.toJson();
    auto *p_reply = mp_networkAccessManager->post(request, jsonData);

    m_requestMap.insert(p_reply, i_callback);
}

//-----------------------------------------------------------------------------
void HttpClient::onResponseReceived(QNetworkReply *ip_reply)
{
    if (ip_reply->error() == QNetworkReply::NoError) {
        auto callback = m_requestMap.value(ip_reply);
        callback(ip_reply->readAll());
    } else {
        qDebug() << "Error in response: " << ip_reply->errorString();
    }

    m_requestMap.remove(ip_reply);
    ip_reply->deleteLater();
}

} // namespace Communication
