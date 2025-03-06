#include "wifi_client.h"

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// ./client_app.exe --host 127.0.0.1 --port 8080

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Client for the HTTP server");

    // add the help option (automatically adds --help to the options)
    parser.addHelpOption();

    parser.addOption({"host", "Host address for the client", "host", "127.0.0.1"});
    parser.addOption({"port", "Port for the client", "port", "8080"});
    parser.process(app);

    QString host = parser.value("host");
    int port = parser.value("port").toInt();

    // create Http Wi-Fi client
    WifiHttpClient wifiClient(host, port);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("wifiClient", &wifiClient);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
