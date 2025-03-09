#include "client.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtGlobal>
#include <QUrlQuery>

namespace Communication {

//-----------------------------------------------------------------------------
HttpClient::HttpClient(QObject* ip_parent /*= nullptr*/)
    : QObject(ip_parent)
    , mp_networkAccessManager(new QNetworkAccessManager(this)){}

//-----------------------------------------------------------------------------
HttpClient::HttpClient(const QString &i_hostName, quint16 i_port, QObject *ip_parent /*=nullptr*/)
    : QObject(ip_parent)
    , m_baseUrl(QString("http://%1:%2").arg(i_hostName).arg(i_port))
    , mp_networkAccessManager(new QNetworkAccessManager(this))
{}

//-----------------------------------------------------------------------------
void HttpClient::setBaseUrl(const QString& i_baseUrl) {
    m_baseUrl = i_baseUrl;
}

//-----------------------------------------------------------------------------
void HttpClient::sendGetRequest(const QString &i_endpoint,
                                std::function<void(QNetworkReply *)> i_callback)
{
    QNetworkRequest request(m_baseUrl + i_endpoint);
    auto *p_reply = mp_networkAccessManager->get(request);

    m_requestMap.insert(p_reply, i_callback);
    connect(p_reply, &QNetworkReply::finished, this, &HttpClient::onFinished);
}

//-----------------------------------------------------------------------------
void HttpClient::sendPostRequest(const QString &i_endpoint,
                                 const QByteArray& i_data,
                                 std::function<void(QNetworkReply *)> i_callback)
{
    QNetworkRequest request(m_baseUrl + i_endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *p_reply = mp_networkAccessManager->post(request, i_data);
    m_requestMap.insert(p_reply, i_callback);
    connect(p_reply, &QNetworkReply::finished, this, &HttpClient::onFinished);
}

//-----------------------------------------------------------------------------
void HttpClient::onFinished() {
    auto* p_reply = qobject_cast<QNetworkReply*>(sender());
    if (p_reply->error() == QNetworkReply::NoError)
        qDebug() << "Response received: OK";
    else
        qDebug() << "HTTP Error:" << p_reply->errorString();

    // invoke callback if registered
    auto callback = m_requestMap.value(p_reply);
    if (callback)
        callback(p_reply);

    m_requestMap.remove(p_reply);
    p_reply->deleteLater();
}

} // namespace Communication
