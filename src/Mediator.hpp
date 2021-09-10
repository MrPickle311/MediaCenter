#pragma once

#include <QObject>
#include <memory>

//@brief This is base class for each mediator which is first layer fo a module.
//       Objects of this class are ... [finish it later] 
class IMediator : public QObject
{
    Q_OBJECT
public:
    virtual ~IMediator(){}
    explicit IMediator(QObject *parent = nullptr);
public:
    virtual QStringList queryAbout(const QString& command, QStringList args) = 0;
signals:
    void requestAction(const QString& requestedAction , QStringList args = {});
    void requestUIAction(const QString& action);
};

using IMediatorPtr =  std::shared_ptr<IMediator>;

class Mediator : public IMediator
{
public:
    virtual QStringList queryAbout(const QString& command, QStringList args);
};