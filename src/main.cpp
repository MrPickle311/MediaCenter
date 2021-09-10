#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVariantMap>
#include <QQmlContext>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //Notification about C++ environment
    auto context {new QQmlContext{engine.rootContext(),&engine}};
    QString cpp_env{"CPP"};
    context->setContextProperty("env",QVariant::fromValue(cpp_env) );

    const QUrl url(QStringLiteral("qrc:/script/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);



    return app.exec();
}
