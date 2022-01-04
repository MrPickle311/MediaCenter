#include "Caller.hpp"
#include <QCoreApplication>
#include <QTimer>
#include <iostream>

static constexpr auto service_name{"org.caller"};
static constexpr auto remote_service_name{"org.receiver"};

Caller::Caller()
    : QObject{nullptr}, receceiver_iface_{remote_service_name, "/receiver", "",
                                          QDBusConnection::sessionBus()} {
  if (receceiver_iface_.connection().isConnected()) {
    std::cout << "Receiver interface is valid!\n";
  }
}

void Caller::coughtReply(QString msg) {
  std::cout << "Caller cought a message : " << msg.toStdString() << '\n';
  //   emit query(msg);
  //   std::cout << "done!\n";
}

void Caller::call() {
  std::cout << "Timer works!\n";
  receceiver_iface_.call("coughtReply", "Hello!");
}

void Caller::blockingCall() {
  QDBusReply<QString> res = receceiver_iface_.call("callForString", "Hello ");
  std::cout << res.value().toStdString() << '\n';
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  Caller caller;

  QDBusConnection::sessionBus().registerService(service_name);
  QDBusConnection::sessionBus().registerObject("/caller", &caller,
                                               QDBusConnection::ExportAllSlots);

  QTimer::singleShot(100, &caller, &Caller::blockingCall);
  return app.exec();
}