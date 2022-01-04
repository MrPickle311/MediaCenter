#pragma once

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <memory>

class Receiver : public QObject {
  Q_OBJECT;

public:
  Receiver();

private:
  QDBusInterface caller_iface_;
public slots:
  Q_NOREPLY void coughtReply(QString msg);
  QString callForString(const QString &args);
};