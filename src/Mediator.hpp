#pragma once

#include <QObject>
#include <memory>
#include "Matcher.hpp"

//@brief This is base class for each mediator which is first layer fo a module.
//       Objects of this class are ... [finish it later] 
class IMediator : public QObject
{
    Q_OBJECT
public:
    virtual ~IMediator(){}
    explicit IMediator(QObject *parent = nullptr);
public:
    virtual QStringList queryAbout(const QString& command, const QStringList& args) = 0;
signals:
    void requestAction(const QString& requestedAction , const QStringList& args = {});
    void requestUIAction(const QString& action);// [move it to another class]
};

using IMediatorPtr =  std::shared_ptr<IMediator>;

class MediatorSubsystems
{
private:
    Matcher                          matcher_;
    std::map<QString , IMediatorPtr> subsystems_;
public:
    MediatorSubsystems();
public:
    void addSubsystem(const QString& subsys_name, IMediatorPtr subsystem);
    IMediatorPtr& getSubsystem(const QString& command) noexcept(false);
    void addSubsystemBinding( const QString& subsystem_name , const QString& binding_key);
};

using MediatorSubsystemsPtr = std::shared_ptr<MediatorSubsystems>;

class Mediator : public IMediator
{
    friend class MediatorBuilder;
protected:
    Mediator();
protected:
    std::shared_ptr<MediatorSubsystems> subsystems_;
protected:
    void redirectRequestAction(const QString& action , const QStringList& args);
public:
    virtual QStringList queryAbout(const QString& command, const QStringList& args) override ;
};

using MediatorPtr = std::shared_ptr<Mediator>;

class MediatorBuilder
{
protected:
    MediatorSubsystemsPtr subsystems_;
protected:
    void resetSubsystems();
public:
    MediatorBuilder():
        subsystems_{new MediatorSubsystems}
    {}
public:
    MediatorBuilder& addSubsystem(const QString& subsystem_name , IMediatorPtr subsystem);
    MediatorBuilder& addSubsystemBinding(const QString& subsystem_name , const QString& binding_key);
    MediatorPtr      build();
};