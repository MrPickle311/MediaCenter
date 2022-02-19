#include <AudioPageConnector.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    modules::AudioPageConnector connector_{ &app };

    qmlRegisterSingletonInstance(
        "connectors.AudioBackend", 1, 0, "AudioBackend", &connector_);

    const QUrl url(QStringLiteral("qrc:/script/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}
