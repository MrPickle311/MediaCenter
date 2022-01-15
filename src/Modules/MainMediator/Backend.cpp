#include "Backend.hpp"


Backend::Backend(uint threads_count) :
    task_manager_{threads_count}
{}

std::future<QStringList> Backend::makeQuery(const QString& command, const QStringList& args)
{
    return task_manager_.addTask(
        [&]
        {
            return Mediator::queryAbout(command , args);
        });
}

QStringList Backend::queryAbout(const QString& command, const QStringList& args)
{
    return makeQuery(command , args).get();
}

//builder

BackendBuilder& BackendBuilder::setThreadsCount(uint th_count) 
{
    this->threads_count_ = th_count;
    return *this;
}

BackendPtr BackendBuilder::buildBackend() 
{
    BackendPtr backend{new Backend{threads_count_}};

    backend->subsystems_ = std::move(this->subsystems_);
    resetSubsystems();

    return backend;
}