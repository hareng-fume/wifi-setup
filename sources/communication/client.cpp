#include "client.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtGlobal>

namespace Communication {

//-----------------------------------------------------------------------------
HttpClient::HttpClient(const QString &i_hostName, quint16 i_port,
                       QObject *ip_parent /*=nullptr*/)
    : QObject(ip_parent),
      m_baseUrl(QString("http://%1:%2").arg(i_hostName).arg(i_port)),
      mp_networkAccessManager(new QNetworkAccessManager(this)) {}

//-----------------------------------------------------------------------------
void HttpClient::sendRequest(HttpMethod i_method, const QString &i_endpoint,
                             const QByteArray &i_data,
                             TReplyCallback i_callback) {
  QNetworkRequest request(m_baseUrl + i_endpoint);
  QNetworkReply *p_reply = nullptr;

  switch (i_method) {
  case HttpMethod::GET:
    p_reply = mp_networkAccessManager->get(request);
    break;
  case HttpMethod::POST:
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    p_reply = mp_networkAccessManager->post(request, i_data);
    break;
  default:
    qWarning() << "WARNING: Unsupported HTTP method";
    return;
  }
  m_requestMap.insert(p_reply, i_callback);
  connect(p_reply, &QNetworkReply::finished, this, &HttpClient::onFinished);
}

//-----------------------------------------------------------------------------
void HttpClient::onFinished() {
  auto *p_reply = qobject_cast<QNetworkReply *>(sender());
  auto callback = m_requestMap.value(p_reply);
  if (callback)
    callback(p_reply);

  m_requestMap.remove(p_reply);
  p_reply->deleteLater();
}

} // namespace Communication
