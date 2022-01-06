#include "Receiver.hpp"
#include <QCoreApplication>
#include <iostream>

static constexpr auto service_name{"org.receiver"};
static constexpr auto remote_service_name{"org.caller"};

Receiver::Receiver()
    : QObject{nullptr}, caller_iface_{remote_service_name, "/", "",
                                      QDBusConnection::sessionBus()} {
  connect(this, &Receiver::testSignal, this, &Receiver::testSlot);
}

void Receiver::coughtReply(QString msg) {
  std::cout << "Receiver cought a reply! : " << msg.toStdString() << '\n';
  auto sth = caller_iface_.call("coughtReply", "Hello!");
  emit testSignal("SIGNAL!\n");
}

QString Receiver::callForString(const QString &args) {
  return args + " World!";
}

void Receiver::testSlot(QString msg) {
  std::cout << msg.toStdString() << std::endl;
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  QDBusConnection::sessionBus().registerService(service_name);
  Receiver receiver;
  QDBusConnection::sessionBus().registerObject("/", &receiver,
                                               QDBusConnection::ExportAllSlots);

  return app.exec();
}