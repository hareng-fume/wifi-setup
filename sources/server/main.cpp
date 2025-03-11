#include "wifi_server.h"

#include <communication/wifi_api.h>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Communication;

namespace _Details {

//-----------------------------------------------------------------------------
static QJsonObject loadJson(const QString &i_path) {
  QFile file(i_path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCritical() << "ERROR: Failed to open file: " << i_path;
    return {};
  }

  auto jsonData = file.readAll();
  auto jsonDoc = QJsonDocument::fromJson(jsonData);
  if (!jsonDoc.isObject()) {
    qCritical() << "ERROR: Invalid JSON format in file: " << i_path;
    return {};
  }

  return jsonDoc.object();
}
} // namespace _Details

// <app>.exe --host 127.0.0.1 --port 8080 --config ./settings.json
int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("HTTP Server for WiFi Management");
  parser.addHelpOption();

  parser.addOption({"config", "Path to the settings JSON file", "config"});
  parser.addOption(
      {"host", "Host address for the server", "host", "127.0.0.1"});
  parser.addOption({"port", "Port for the server", "port", "8080"});
  parser.process(app);

  if (!parser.isSet("config")) {
    qCritical() << "ERROR: Missing required argument: --config <path_to_json>";
    parser.showHelp(EXIT_FAILURE);
  }

  auto configPath = parser.value("config");
  auto settings = _Details::loadJson(configPath);
  if (settings.isEmpty()) {
    return EXIT_FAILURE;
  }

  QString host = parser.value("host");
  int port = parser.value("port").toInt();

  WifiHttpServer server(&app);
  server.loadWifiCredentials(settings);
  server.start(host, port);
  if (!server.isListening())
    return EXIT_FAILURE;

  return app.exec();
}
