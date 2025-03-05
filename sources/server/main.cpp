#include "wifi_server.h"
#include <communication/wifi_api.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>

using namespace Communication;

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    WifiHttpServer server;
    server.start();

    return a.exec();
}
