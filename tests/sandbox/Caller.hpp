#pragma once

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

class Caller : public QObject {
  Q_OBJECT;

public:
  Caller();

private:
  QDBusInterface receceiver_iface_;
public slots:
  Q_NOREPLY void coughtReply(QString msg);
  void call();
  void blockingCall();
};