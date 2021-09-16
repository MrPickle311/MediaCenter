#include "Mediator.hpp"
#include <iostream>

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

MediatorSubsystems::MediatorSubsystems():
    matcher_{0}
{}

void MediatorSubsystems::addSubsystem(const QString& subsys_name, IMediatorPtr subsystem)
{
    subsystems_[subsys_name] = subsystem;
}

IMediatorPtr& MediatorSubsystems::getSubsystem(const QString& command) noexcept(false)
{
    return subsystems_.at(command);
    // return subsystems_.at(matcher_.extractKey(command));
}

void MediatorSubsystems::addSubsystemBinding( const QString& subsystem_name , const QString& binding_key)
{
    subsystems_[binding_key] = subsystems_.at(subsystem_name);
}

void MediatorSubsystems::setDesiredParserPos(int desired_parser_pos) 
{
    this->matcher_.setDesiredParsedPos(desired_parser_pos);
}

Mediator::Mediator()
{
    QObject::connect(this , &Mediator::requestAction ,
                     this , &Mediator::redirectRequestAction);
}

void Mediator::redirectRequestAction(const QString& action , const QStringList& args) 
{
    try//if command is correct resend request
    {
        #pragma message "You call requestAction() directly , change to emit requestAction()!! " 
        subsystems_->getSubsystem(action)->requestAction(action , args);
    }
    catch(const std::out_of_range& e)
    {
        std::cout << "\n Backend::redirectRequestAction : Requested action not found \n ";
        //nothing will happen otherwise
    }
    catch(...)
    {
        std::cout << "\n Backend::redirectRequestAction : Unexpected error \n ";
    }
}

QStringList Mediator::queryAbout(const QString& command, const QStringList& args) 
{
    try
    {
        return subsystems_->getSubsystem(command)->queryAbout(command ,args);
    }
    catch(const std::out_of_range& e)
    {
        return QStringList{"WrongCmd"};
    }
}

void SubsystemProxy::setSubsystems(MediatorSubsystemsPtr subsys) 
{
    this->subsystems_ = subsys;
}

// builder

MediatorBuilder::MediatorBuilder():
        subsystems_{std::make_shared<MediatorSubsystems>()}
{
    proxy_.setSubsystems(subsystems_);
}

void MediatorBuilder::resetSubsystems() 
{
    this->subsystems_.reset(new MediatorSubsystems);
    proxy_.setSubsystems(subsystems_);
}

MediatorBuilder& MediatorBuilder::addSubsystem(const QString& subsystem_name , std::shared_ptr<IMediator> subsystem)
{
    subsystems_->addSubsystem(subsystem_name , subsystem);
    return *this;
}

SubsystemProxy& MediatorBuilder::at(const QString& subsystem_name) 
{
    proxy_.current_subsystem_ = subsystem_name;
    return proxy_;
}

SubsystemProxy& SubsystemProxy::addBinding(const QString& command) 
{
    subsystems_->addSubsystemBinding(current_subsystem_ , command);
    return *this;
}

MediatorPtr MediatorBuilder::build() 
{
    MediatorPtr mediator{new Mediator};

    mediator->subsystems_ = std::move(this->subsystems_);
    resetSubsystems();

    return mediator;
}