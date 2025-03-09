#pragma once

#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QString>

#include <functional>

class QNetworkReply;
class QNetworkAccessManager;

namespace Communication {

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject* ip_parent = nullptr);
    HttpClient(const QString &i_hostName, quint16 i_port, QObject *ip_parent = nullptr);

    void setBaseUrl(const QString& i_baseUrl);

    void sendGetRequest(const QString &i_endpoint, std::function<void(QNetworkReply *)> i_callback);
    void sendPostRequest(const QString &i_endpoint,
                         const QByteArray& i_data,
                         std::function<void(QNetworkReply *)> i_callback);

private slots:
    void onFinished();

private:
    QString m_baseUrl; // store predefined host and port
    QNetworkAccessManager *mp_networkAccessManager;

    // map to store replies and their associated request IDs
    QMap<QNetworkReply *, std::function<void(QNetworkReply *)>> m_requestMap;
};

} // namespace Communication
