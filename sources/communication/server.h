#pragma once

#include <QMap>
#include <QString>
#include <QTcpServer>


namespace Communication {

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HttpServer(QObject *ip_parent = nullptr);

    void start(const QString &i_address, quint16 i_port = 8080);
    void route(const QString &i_endpoint,
               std::function<void(QTcpSocket *, const QString &)> i_callback);
    void sendResponse(QTcpSocket *ip_socket,
                      const QString &i_status,
                      const QByteArray &i_responseData);

protected:
    void incomingConnection(qintptr i_socketDescriptor) override;

private slots:
    void onNewConnection();
    void onDisconnected();
    void processRequest(QTcpSocket *ip_socket);

private:
    QMap<QString, std::function<void(QTcpSocket *, const QString &)>> m_routes;
    QHash<QTcpSocket*, QByteArray> m_buffer;
};

} // namespace Communication
