#include "client.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
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
                                std::function<void(QNetworkReply *)> i_callback)
{
    QNetworkRequest request(m_baseUrl + i_endpoint);
    auto *p_reply = mp_networkAccessManager->get(request);

    m_requestMap.insert(p_reply, i_callback);
}

//-----------------------------------------------------------------------------
void HttpClient::sendPostRequest(const QString &i_endpoint,
                                 QJsonObject i_jsonObj,
                                 std::function<void(QNetworkReply *)> i_callback)
{
    QNetworkRequest request(m_baseUrl + i_endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(i_jsonObj);
    QByteArray jsonData = doc.toJson();
    auto *p_reply = mp_networkAccessManager->post(request, jsonData);

    m_requestMap.insert(p_reply, i_callback);
}

//-----------------------------------------------------------------------------
void HttpClient::onResponseReceived(QNetworkReply *ip_reply)
{
    if (m_requestMap.contains(ip_reply)) {
        m_requestMap[ip_reply](ip_reply);
        m_requestMap.remove(ip_reply);
    } else {
        qDebug() << "ERROR: no callback registered";
        ip_reply->deleteLater();
    }
}

} // namespace Communication
