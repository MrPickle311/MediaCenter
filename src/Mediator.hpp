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
    virtual ~IMediator() = default;
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
    CommandMatcher                   matcher_;
    std::map<QString , IMediatorPtr> subsystems_;
public:
    MediatorSubsystems();
public:
    void addSubsystem(const QString& subsys_name, IMediatorPtr subsystem);
    IMediatorPtr& getSubsystem(const QString& command) noexcept(false);
    void addSubsystemBinding( const QString& subsystem_name , const QString& command);
    void setDesiredParserPos(int desired_parser_pos);
};

using MediatorSubsystemsPtr = std::shared_ptr<MediatorSubsystems>;

class Mediator : public IMediator
{
    friend class MediatorBuilder;
public:
    Mediator();
protected:
    std::shared_ptr<MediatorSubsystems> subsystems_;
protected:
    void redirectRequestAction(const QString& action , const QStringList& args);
public:
    virtual QStringList queryAbout(const QString& command, const QStringList& args) override ;
};

using MediatorPtr = std::shared_ptr<Mediator>;

class SubsystemProxy
{
    friend class MediatorBuilder;
private:
    void setSubsystems(MediatorSubsystemsPtr subsys);
private:
    MediatorSubsystemsPtr subsystems_;
    QString               current_subsystem_;
public:
    SubsystemProxy& addBinding(const QString& command);
};

class MediatorBuilder
{
protected:
    MediatorSubsystemsPtr subsystems_;
    SubsystemProxy        proxy_;
protected:
    void resetSubsystems();
public:
    MediatorBuilder();
public:
    MediatorBuilder& addSubsystem(const QString& subsystem_name , IMediatorPtr subsystem);
    SubsystemProxy&  at(const QString& subsystem_name );
    MediatorPtr      build();
};

//double builder

class SystemBuilder
{
public:
    SystemBuilder(){}
};