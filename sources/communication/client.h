#pragma once

#include "wifi_api.h"

#include <QMap>
#include <QObject>
#include <QString>

#include <functional>

class QNetworkReply;
class QNetworkAccessManager;

namespace Communication {

class HttpClient : public QObject {
  Q_OBJECT

  using TReplyCallback = std::function<void(QNetworkReply *)>;

public:
  HttpClient(const QString &i_hostName, quint16 i_port,
             QObject *ip_parent = nullptr);

  void sendRequest(HttpMethod i_method, const QString &i_endpoint,
                   const QByteArray &i_data = QByteArray(),
                   TReplyCallback i_callback = nullptr);

private slots:
  void onFinished();

private:
  QString m_baseUrl;
  QNetworkAccessManager *mp_networkAccessManager;

  QMap<QNetworkReply *, TReplyCallback> m_requestMap;
};

} // namespace Communication
