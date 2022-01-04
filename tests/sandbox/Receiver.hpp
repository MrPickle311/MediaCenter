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
  std::unique_ptr<QDBusInterface> caller_iface_;
  QDBusServiceWatcher watcher_;
public slots:
  Q_NOREPLY void coughtReply(QString msg);
  void call();
  void initInterface(const QString &service);
signals:
  void query(QString msg);
};