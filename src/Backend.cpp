#include "Backend.hpp"
#include <regex>

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

IProxy::IProxy(QObject *parent):
    QObject(parent)
{}

Backend::Backend(uint threads_count) :
    task_manager_{threads_count}
{}

// backend

std::shared_ptr<IMediator>& Backend::redirect(QString command)
{
    return settings_backend_;
}

std::future<QStringList> Backend::makeQuery(QString command, QStringList args)
{
    return task_manager_.addTask(
        [&]
        {
            return redirect(command)->queryAbout(command ,args);
        });
}

QStringList Backend::queryAbout(QString command, QStringList args)
{
    return makeQuery(std::move(command) , std::move(args)).get();
}

// builder

std::shared_ptr<Backend> BackendBuilder::build() 
{
    std::shared_ptr<Backend> backend{new Backend{threads_count_}};
    backend->data_backend_ = this->data_backend_;
    backend->settings_backend_ = this->settings_backend_;
    backend->environment_backend_ = this->env_backend_;
    return backend;
}

BackendBuilder& BackendBuilder::setDataBackendDependency(std::shared_ptr<IMediator> data_backend) 
{
    this->data_backend_ = data_backend;
    return *this;
}

BackendBuilder& BackendBuilder::setEnvironmentDependency(std::shared_ptr<IMediator> env_backend) 
{
    this->env_backend_ = env_backend;
    return *this;
}

BackendBuilder& BackendBuilder::setSettingsDependency(std::shared_ptr<IMediator> settings_backend) 
{
    this->settings_backend_ = settings_backend;
    return *this;
}

BackendBuilder& BackendBuilder::setThreadsCount(uint th_count) 
{
    this->threads_count_ = th_count;
    return *this;
}
