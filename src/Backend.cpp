#include "Backend.hpp"

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

IProxy::IProxy(QObject *parent):
    QObject(parent)
{}

// BackendSubsystems

BackendSubsystems::BackendSubsystems():
        matcher_{"[A-Z]{1}[a-z]+"}
    {}

void BackendSubsystems::addSubsystem(const QString& subsys_name, IMediatorPtr subsystem)
{
    subsystems_[subsys_name] = subsystem;
}

IMediatorPtr& BackendSubsystems::getSubsystem(const QString& command) noexcept(false)
{
    return subsystems_.at(matcher_.extractKey(command));
}

void BackendSubsystems::addSubsystemBinding( const QString& subsystem_name , const QString& binding_key)
{
    subsystems_[binding_key] = subsystems_.at(subsystem_name);
}

// backend

Backend::Backend(uint threads_count) :
    task_manager_{threads_count}
{
    QObject::connect(this , &Backend::requestAction ,
                     this , &Backend::redirectRequestAction);
}


// !!! MAKE CONST REF EVERYWHERE U CAN !!!
std::future<QStringList> Backend::makeQuery(const QString& command, QStringList args)
{
    return task_manager_.addTask(
        [&]
        {
            try
            {
                return subsystems_->getSubsystem(command)->queryAbout(command ,args);
            }
            catch(const std::out_of_range& e)
            {
                return QStringList{"WrongCmd"};
            }
        });
}

void Backend::redirectRequestAction(const QString& action , QStringList args) 
{
    try//if command is correct resend request
    {
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

QStringList Backend::queryAbout(const QString& command, QStringList args)
{
    return makeQuery(command , args).get();
}

void BackendBuilder::resetSubsystems() 
{
    this->subsystems_.reset(new BackendSubsystems);
}

// builder

std::shared_ptr<Backend> BackendBuilder::build() 
{
    std::shared_ptr<Backend> backend{new Backend{threads_count_}};

    backend->subsystems_ = std::move(this->subsystems_);
    resetSubsystems();

    return backend;
}

BackendBuilder& BackendBuilder::addSubsystem(const QString& subsystem_name , std::shared_ptr<IMediator> subsystem)
{
    subsystems_->addSubsystem(subsystem_name , subsystem);
    return *this;
}

BackendBuilder& BackendBuilder::addSubsystemBinding(const QString& subsystem_name , const QString& binding_key) 
{
    subsystems_->addSubsystemBinding(subsystem_name , binding_key);
    return *this;
}

BackendBuilder& BackendBuilder::setThreadsCount(uint th_count) 
{
    this->threads_count_ = th_count;
    return *this;
}
