#pragma once

#include "../SystemNode/SystemNode.hpp"
#include "Matcher.hpp"

#include <QObject>
#include <memory>

namespace common
{

// forward delcarations
// class MediatorSubsystems;

class Mediator : public common::IBehaviourController
{
    friend class MediatorBuilder;
    friend class SystemConfigurator;

protected:
    using SendSignalFunctionType = decltype(ISystemNode::sendSignal);
    using RequestDataFunctionType = decltype(ISystemNode::requestData);

protected:
    SendSignalFunctionType signal_sender_;
    RequestDataFunctionType data_requester_;
    IMatcher& matcher_;

public:
    Mediator(SendSignalFunctionType signal_sender,
             RequestDataFunctionType data_requester,
             IMatcher& matcher);

protected:
    QString redirectRequestAction(QJsonDocument& message);

public:
    virtual void onCoughtSignal(QJsonDocument message) override;
    virtual QJsonDocument onRequestedData(QJsonDocument command) override;
};

using MediatorPtr = std::shared_ptr<Mediator>;

// TODO: change meaning of this class
class MediatorSubsystems
{
private:
    CommandMatcher matcher_;
    std::map<QString, MediatorPtr> subsystems_;

public:
    MediatorSubsystems();

public:
    void addSubsystem(const QString& subsys_name, MediatorPtr subsystem);
    MediatorPtr& getSubsystem(const QString& command) noexcept(false);
    void addSubsystemBinding(const QString& subsystem_name,
                             const QString& command);
    void setDesiredParserPos(int desired_parser_pos);
};

using MediatorSubsystemsPtr = std::shared_ptr<MediatorSubsystems>;


class SubsystemProxy
{
    friend class MediatorBuilder;
    friend class SystemConfigurator;

private:
    void setSubsystems(MediatorSubsystemsPtr subsys);
    void setTargetToBind(const QString& target_name);

private:
    MediatorSubsystemsPtr subsystems_;
    QString target_to_bind_;

public:
    SubsystemProxy& addBinding(const QString& command);
};

class MediatorBuilder
{
protected:
    MediatorSubsystemsPtr subsystems_;
    SubsystemProxy proxy_;

protected:
    void resetSubsystems();

public:
    MediatorBuilder();

public:
    MediatorBuilder& addSubsystem(const QString& subsystem_name,
                                  MediatorPtr subsystem);
    SubsystemProxy& at(const QString& subsystem_name);
    MediatorPtr build();
};

// double builder

class SystemConfigurator
{
private:
    std::map<QString, MediatorPtr> mediators_;

    std::map<QString, MediatorSubsystemsPtr> mediators_subsystems_;

    QString current_mediator;


    SubsystemProxy proxy_;

private:
    void connection_engine(const QString& first, const QString& second);

public:
    void addMediator(MediatorPtr mediator, const QString& subsystem_name);
    void connect(const QString& first, const QString& second);
    SystemConfigurator& from(const QString& subsystem_name);
    SubsystemProxy& to(const QString& subsystem_name);
};

} // namespace common