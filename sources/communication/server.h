#pragma once

#include <QHash>
#include <QMap>
#include <QString>
#include <QTcpServer>

class QTcpSocket;

namespace Communication {

class HttpServer : public QTcpServer {
  Q_OBJECT

  using TRequestCallback = std::function<void(QTcpSocket *, const QString &)>;

public:
  explicit HttpServer(QObject *ip_parent = nullptr);

  void start(const QString &i_address, quint16 i_port);
  void route(const QString &i_endpoint, TRequestCallback i_callback);
  void sendResponse(QTcpSocket *ip_socket, const QString &i_status,
                    const QByteArray &i_data);

protected:
  void incomingConnection(qintptr i_socketDescriptor) override;

private slots:
  void onNewConnection();
  void onDisconnected();
  void onReadyRead();

private:
  QMap<QString, TRequestCallback> m_routes;
  QHash<QTcpSocket *, QByteArray> m_buffer;
};

} // namespace Communication
