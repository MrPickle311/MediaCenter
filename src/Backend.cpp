#include "Backend.hpp"

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

IProxy::IProxy(QObject *parent):
    QObject(parent)
{}

Backend::Backend(int threads_count) :
    task_manager_{threads_count}
{}

// backend

QStringList Backend::queryAbout(QString sender, QString what, QStringList args)
{
    //I HAVE FINISHED HERE

    
    return {};
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

BackendBuilder& BackendBuilder::setThreadsCount(int th_count) 
{
    this->threads_count_ = th_count;
    return *this;
}
