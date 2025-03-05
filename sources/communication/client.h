#ifndef CLIENT_H
#define CLIENT_H

#include <QMap>
#include <QObject>
#include <QString>

class QNetworkReply;
class QNetworkAccessManager;

namespace Communication {

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *ip_parent = nullptr);

    void sendGetRequest(const QString &i_url, const QString &i_requestId);
    void sendPostRequest(const QString &i_url,
                         const QJsonObject &i_data,
                         const QString &i_requestId);

private slots:
    void onResponseReceived(QNetworkReply *ip_reply);
    void onReadyRead();

private:
    QNetworkAccessManager *mp_networkAccessManager;

    // map to store replies and their associated request IDs
    QMap<QNetworkReply *, QString> m_requestMap;
};

} // namespace Communication

#endif // CLIENT_H
