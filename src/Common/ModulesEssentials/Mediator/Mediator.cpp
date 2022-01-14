#include "Mediator.hpp"

#include <iostream>

namespace common
{

/**

MediatorSubsystems::MediatorSubsystems() : matcher_{ 0 } {}

void MediatorSubsystems::addSubsystem(const QString& subsys_name,
                                      IMediatorPtr subsystem)
{
    subsystems_[subsys_name] = subsystem;
}

IMediatorPtr&
MediatorSubsystems::getSubsystem(const QString& command) noexcept(false)
{
    return subsystems_.at(command);
    // return subsystems_.at(matcher_.extractKey(command));
}

void MediatorSubsystems::addSubsystemBinding(const QString& subsystem_name,
                                             const QString& binding_key)
{
    subsystems_[binding_key] = subsystems_.at(subsystem_name);
}

void MediatorSubsystems::setDesiredParserPos(int desired_parser_pos)
{
    this->matcher_.setDesiredParsedPos(desired_parser_pos);
}

**/

Mediator::Mediator(SendSignalFunctionType signal_sender,
                   RequestDataFunctionType data_requester,
                   IMatcher& matcher) :
    signal_sender_{ signal_sender },
    data_requester_{ data_requester },
    matcher_{ matcher }
{}

QString Mediator::redirectRequestAction(QJsonDocument& message)
{
    return matcher_.extractKey(message);
}

void Mediator::onCoughtSignal(QJsonDocument message)
{
    auto target{ redirectRequestAction(message) };
    signal_sender_(target, message);
}

QJsonDocument Mediator::onRequestedData(QJsonDocument command)
{
    auto target{ redirectRequestAction(command) };
    return data_requester_(target, command);
}

// builder
/**
MediatorBuilder::MediatorBuilder() :
    subsystems_{ std::make_shared<MediatorSubsystems>() }
{
    proxy_.setSubsystems(subsystems_);
}

void MediatorBuilder::resetSubsystems()
{
    this->subsystems_.reset(new MediatorSubsystems);
    proxy_.setSubsystems(subsystems_);
}

MediatorBuilder&
MediatorBuilder::addSubsystem(const QString& subsystem_name,
                              std::shared_ptr<IMediator> subsystem)
{
    subsystems_->addSubsystem(subsystem_name, subsystem);
    return *this;
}

SubsystemProxy& MediatorBuilder::at(const QString& subsystem_name)
{
    proxy_.target_to_bind_ = subsystem_name;
    return proxy_;
}

void SubsystemProxy::setSubsystems(MediatorSubsystemsPtr subsys)
{
    this->subsystems_ = subsys;
}

void SubsystemProxy::setTargetToBind(const QString& target_name)
{
    this->target_to_bind_ = target_name;
}

SubsystemProxy& SubsystemProxy::addBinding(const QString& command)
{
    subsystems_->addSubsystemBinding(target_to_bind_, command);
    return *this;
}

MediatorPtr MediatorBuilder::build()
{
    MediatorPtr mediator{ new Mediator };

    mediator->subsystems_ = std::move(this->subsystems_);
    resetSubsystems();

    return mediator;
}

void SystemConfigurator::connection_engine(const QString& first,
                                           const QString& second)
{
    mediators_subsystems_.at(first)->addSubsystem(second, mediators_.at(second));
}

void SystemConfigurator::addMediator(MediatorPtr mediator,
                                     const QString& subsystem_name)
{
    mediators_[subsystem_name] = mediator;
    mediators_subsystems_[subsystem_name] = mediator->subsystems_;
}

void SystemConfigurator::connect(const QString& first, const QString& second)
{
    connection_engine(first, second);
    connection_engine(second, first);
}

SystemConfigurator& SystemConfigurator::from(const QString& subsystem_name)
{
    proxy_.setSubsystems(mediators_subsystems_.at(subsystem_name));
    return *this;
}

SubsystemProxy& SystemConfigurator::to(const QString& subsystem_name)
{
    proxy_.setTargetToBind(subsystem_name);
    return proxy_;
}
**/
} // namespace common