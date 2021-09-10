#pragma once

#include <QObject>
#include <memory>

class IProxy : public QObject
{
    Q_OBJECT
public:
    explicit IProxy(QObject *parent = nullptr);
public slots:
    virtual void requestAction(const QString& action , QStringList args) = 0;
    virtual QStringList requestData(const QString& what) = 0;
};

using IProxyPtr =  std::shared_ptr<IProxy>;