#include "client.h"

#include <QDebug>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QtGlobal>

namespace Communication {

//-----------------------------------------------------------------------------
HttpClient::HttpClient(QObject *ip_parent /*=nullptr*/)
    : QObject(ip_parent)
    , mp_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(mp_networkAccessManager,
            &QNetworkAccessManager::finished,
            this,
            &HttpClient::onResponseReceived);
}

//-----------------------------------------------------------------------------
void HttpClient::sendGetRequest(const QString &i_url, const QString &i_requestId)
{
    QNetworkRequest request(i_url);
    auto *p_reply = mp_networkAccessManager->get(request);

    m_requestMap.insert(p_reply, i_requestId);
    connect(p_reply, &QNetworkReply::readyRead, this, &HttpClient::onReadyRead);
}

//-----------------------------------------------------------------------------
void HttpClient::sendPostRequest(const QString &i_url,
                                 const QJsonObject &i_data,
                                 const QString &i_requestId)
{
    QNetworkRequest request(i_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(i_data);
    auto jsonData = doc.toJson();
    auto *p_reply = mp_networkAccessManager->post(request, jsonData);

    m_requestMap.insert(p_reply, i_requestId);
    connect(p_reply, &QNetworkReply::readyRead, this, &HttpClient::onReadyRead);
}

//-----------------------------------------------------------------------------
void HttpClient::onResponseReceived(QNetworkReply *ip_reply)
{
    auto requestId = m_requestMap.value(ip_reply);

    if (ip_reply->error() == QNetworkReply::NoError) {
        // the response is finished, read the full responsonse body
        QByteArray fullData = ip_reply->readAll();
        qDebug() << "Full response for request ID: " << requestId << ", received: " << fullData;
    } else {
        qDebug() << "Error in response for request ID: " << requestId
                 << ", error: " << ip_reply->errorString();
    }

    m_requestMap.remove(ip_reply);
    ip_reply->deleteLater();
}

//-----------------------------------------------------------------------------
void HttpClient::onReadyRead()
{
    QNetworkReply *p_reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(p_reply);

    QByteArray data = p_reply->readAll();

    // TODO: process the incoming data chunk
    qDebug() << "Received data chunk:" << data;
}

} // namespace Communication
