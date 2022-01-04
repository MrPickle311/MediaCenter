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
  receceiver_iface_.asyncCall("coughtReply", "Hello!");
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  Caller caller;

  QDBusConnection::sessionBus().registerObject("/caller", &caller,
                                               QDBusConnection::ExportAllSlots);

  QDBusConnection::sessionBus().registerService(service_name);
  QTimer::singleShot(100, &caller, &Caller::call);
  return app.exec();
}