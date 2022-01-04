#include "Receiver.hpp"
#include <QCoreApplication>
#include <iostream>

static constexpr auto service_name{"org.receiver"};
static constexpr auto remote_service_name{"org.caller"};

Receiver::Receiver()
    : QObject{nullptr}, caller_iface_{nullptr},
      watcher_{remote_service_name, QDBusConnection::sessionBus()} {

  connect(&watcher_, &QDBusServiceWatcher::serviceRegistered, this,
          &Receiver::initInterface);
}

void Receiver::initInterface(const QString &service) {

  std::cout << "Service registered!\n";
  caller_iface_ = std::make_unique<QDBusInterface>(
      remote_service_name, "/caller", "", QDBusConnection::sessionBus());

  if (caller_iface_->isValid()) {
    std::cout << "Caller interface is valid!\n";
  }
}

void Receiver::coughtReply(QString msg) {
  std::cout << "Receiver cought a reply! : " << msg.toStdString() << '\n';
  auto sth = caller_iface_->call("coughtReply", "Hello!");
}

void Receiver::call() {}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  QDBusConnection::sessionBus().registerService(service_name);

  Receiver receiver;
  QDBusConnection::sessionBus().registerObject("/receiver", &receiver,
                                               QDBusConnection::ExportAllSlots);

  return app.exec();
}