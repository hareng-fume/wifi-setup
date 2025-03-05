#ifndef SERVER_H
#define SERVER_H

#include "settings.h"

#include <QTcpServer>
#include <memory>

namespace Communication {

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
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

    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

protected:
    void incomingConnection(qintptr i_socketDescriptor) override;

private:
    std::unique_ptr<ISettings> mp_settings;
};

} // namespace Communication

#endif // SERVER_H
