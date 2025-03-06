#include "wifi_server.h"
#include <communication/wifi_api.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>

using namespace Communication;

namespace _Details {

//-----------------------------------------------------------------------------
static QJsonObject loadJson(const QString &i_path)
{
    QFile file(i_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file: " << i_path;
        return {};
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isObject()) {
        qCritical() << "Invalid JSON format in file: " << i_path;
        return {};
    }

    return jsonDoc.object();
}
} // namespace _Details

// ./server_app.exe --host 127.0.0.1 --port 8080 --config ./settings.json
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("HTTP Server for WiFi Management");

    // add the help option (automatically adds --help to the options)
    parser.addHelpOption();

    parser.addOption({"config", "Path to the settings JSON file", "config"});
    parser.addOption({"host", "Host address for the server", "host", "127.0.0.1"});
    parser.addOption({"port", "Port for the server", "port", "8080"});
    parser.process(app);

    if (!parser.isSet("config")) {
        qCritical() << "Missing required argument: --config <path_json>";
        parser.showHelp(1); // exits with error
    }

    auto configPath = parser.value("config");
    auto settings = _Details::loadJson(configPath);
    if (settings.isEmpty()) {
        qCritical() << "Failed to load config settings.";
        return 1;
    }

    QString host = parser.value("host");
    int port = parser.value("port").toInt();

    WifiHttpServer server(std::move(settings));
    server.start(host, port);

    return app.exec();
}
