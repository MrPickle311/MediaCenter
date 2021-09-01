#include "Backend.hpp"
#include <regex>
#include <map>

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

IProxy::IProxy(QObject *parent):
    QObject(parent)
{}

// backend

Backend::Backend(uint threads_count) :
    task_manager_{threads_count}
{
    addBinding("Search" , data_backend_);
    addBinding("Playlist" , data_backend_);
    addBinding("Mediapaths" , settings_backend_);
}

void Backend::addBinding(QString key , std::shared_ptr<IMediator> target) 
{
    bindings_[key] = target;
}

QString Backend::getTargetKey(QString command)
{
    const static thread_local std::regex matcher{"[A-Z]{1}+[a-z]+"};
    std::string str{command.toStdString()};
    std::smatch sm;

    if(std::regex_search(str , sm , matcher))
    {
        auto g = sm[0].str();
    }
    else return "WrongCmd";
}

std::shared_ptr<IMediator>& Backend::redirect(QString command)
{
    QString target_key{getTargetKey(command)};

    if(bindings_.find(target_key) == bindings_.end())
    {
        throw std::logic_error{"WrongCmd"};
    }

    return bindings_[target_key];
}
// !!! MAKE CONST REF EVERYWHERE U CAN !!!
std::future<QStringList> Backend::makeQuery(QString command, QStringList args)
{
    return task_manager_.addTask(
        [&]
        {
            try
            {
                return redirect(command)->queryAbout(command ,args);
            }
            catch(const std::logic_error& e)
            {
                return QStringList{"WrongCmd"};
            }
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

    backend->data_backend_        = this->data_backend_;
    backend->settings_backend_    = this->settings_backend_;
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
