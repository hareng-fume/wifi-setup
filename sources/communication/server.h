#ifndef SERVER_H
#define SERVER_H

#include "settings.h"

#include <QMap>
#include <QString>
#include <QTcpServer>

#include <memory>

namespace Communication {
namespace Status {
inline static constexpr const char *_OK = "200 OK";
inline static constexpr const char *_UNAUTHORIZED = "401 Unauthorized";
inline static constexpr const char *_BAD_REQUEST = "400 Bad Request";
inline static constexpr const char *_NOT_FOUND = "404 Not Found";
} // namespace Status

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HttpServer(QObject *ip_parent = nullptr);

    template<typename TSettings, typename... TArgs>
    explicit HttpServer(QObject *ip_parent, TArgs &&...i_args)
        : QTcpServer(ip_parent)
        , mp_settings(std::make_unique<TSettings>(std::forward<TArgs>(i_args)...))
    {
        static_assert(std::is_base_of_v<ISettings, TSettings>,
                      "HttpServer doesn't support TSetting");

        if (!listen(QHostAddress::Any, 8080)) {
            qDebug() << "Server failed to start!";
        } else {
            qDebug() << "Server listening on port 8080...";
        }
    }

    void start(quint16 i_port = 8080);
    void route(const QString &i_endpoint,
               std::function<void(QTcpSocket *, const QString &)> i_callback);
    void sendResponse(QTcpSocket *ip_socket,
                      const QString &i_status,
                      const QByteArray &i_responseData);

protected:
    void incomingConnection(qintptr i_socketDescriptor) override;

private slots:
    void handleNewConnection();
    void processRequest(QTcpSocket *ip_socket);

private:
    std::unique_ptr<ISettings> mp_settings;
    QMap<QString, std::function<void(QTcpSocket *, const QString &)>> m_routes;
};

} // namespace Communication

#endif // SERVER_H
