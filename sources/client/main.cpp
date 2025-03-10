#include "wifi_client.h"

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    WifiNetwork::registerMetatypes();

    QCommandLineParser parser;
    parser.setApplicationDescription("HTTP Client for WiFi Management");
    parser.addHelpOption();

    parser.addOption({"host", "Host address for the client", "host", "127.0.0.1"});
    parser.addOption({"port", "Port for the client", "port", "8080"});
    parser.process(app);

    QString host = parser.value("host");
    quint16 port = parser.value("port").toUShort();

    WifiHttpClient wifiClient(host, port, &app);

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
