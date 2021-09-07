#pragma once

#include <QObject>
#include <QVariant>
#include <QThreadPool>
#include <map>
#include <regex>
#include "utilities/TaskManager.hpp"

class IMediator : public QObject
{
    Q_OBJECT
public:
    virtual ~IMediator(){}
    explicit IMediator(QObject *parent = nullptr);
public:
    virtual QStringList queryAbout(QString command, QStringList args) = 0;
signals:
    void requestAction(QString requestedAction , QStringList args = {});
    void requestUIAction(QString action);
};

class IProxy : public QObject
{
    Q_OBJECT
public:
    explicit IProxy(QObject *parent = nullptr);
public slots:
    virtual void requestAction(QString action , QVariantList args) = 0;
    virtual QVariantList requestData(QString what) = 0;
};


class Matcher
{
private:
    const std::regex matcher_body_;
public:
    Matcher(std::string regex_pattern):
        matcher_body_{regex_pattern}
    {}
    QString extractSubsystemKey(const QString command)
    {
        thread_local static std::string str{command.toStdString()};
        thread_local static std::smatch match_results;

        if(std::regex_search(str , match_results  , matcher_body_))
        {
            return QString::fromStdString(match_results[0].str());
        }

        return "WrongCmd";
    }
};

class BackendSubsystems
{
    using IMediatorPtr =  std::shared_ptr<IMediator>;
private:
    Matcher                          matcher_;
    std::map<QString , IMediatorPtr> subsystems_;
public:
    void addSubsystem(const QString& subsys_key, IMediatorPtr subsystem)
    {
        subsystems_[subsys_key] = subsystem;
    }
    IMediatorPtr& getSubsystem(const QString& command) noexcept(false)
    {
        QString target_key{matcher_.extractSubsystemKey(command)};

        if(subsystems_.find(target_key) == subsystems_.end())
        {
            throw std::logic_error{"WrongCmd"};
        }

        return subsystems_[command];
    }
};

class Backend : public IMediator
{
    Q_OBJECT;
    friend class BackendBuilder;
private:
    Backend(uint threads_count);
private:
    TaskManager task_manager_;

    //dependencies
    std::shared_ptr<BackendSubsystems> subsystems_;
private:
    std::future<QStringList> makeQuery(QString command, QStringList args);
    void redirectRequestAction(QString action , QVariantList args);
public slots:
    virtual QStringList queryAbout(QString command, QStringList args) override;
};

class BackendBuilder
{
private:
    uint threads_count_;
public:
    std::shared_ptr<Backend> build();
    BackendBuilder& setDataBackendDependency(std::shared_ptr<IMediator> data_backend);
    BackendBuilder& setEnvironmentDependency(std::shared_ptr<IMediator> env_backend);
    BackendBuilder& setSettingsDependency(std::shared_ptr<IMediator> settings_backend);
    BackendBuilder& setThreadsCount(uint th_count);
};