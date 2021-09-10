#pragma once

#include <QObject>
#include <map>
#include "utilities/TaskManager.hpp"
#include "Matcher.hpp"
#include "Mediator.hpp"

//generic string adapter [MOVE IT SOMEWHERE]
class String
{
private:
    std::string body_;
public:
    String(const std::string& str):
        body_{str}
    {}
    String(const QString& str):
        body_{str.toStdString()}
    {}
    String(const char* str):
        body_{str}
    {}
    operator QString () const
    {
        return QString::fromStdString(body_);
    }
    operator std::string ()
    {
        return body_;
    }
    operator const char* () const
    {
        return body_.c_str();
    }
    String& operator=(const std::string& str)
    {
        this->body_ = str;
        return *this;
    }
    String& operator=(const QString& str)
    {
        this->body_ = str.toStdString();
        return *this;
    }
    String& operator=(const char* str)
    {
        this->body_ = str;
        return *this;
    }
};

class BackendSubsystems
{
private:
    Matcher                          matcher_;
    std::map<QString , IMediatorPtr> subsystems_;
public:
    BackendSubsystems();
    void addSubsystem(const QString& subsys_name, IMediatorPtr subsystem);
    IMediatorPtr& getSubsystem(const QString& command) noexcept(false);
    void addSubsystemBinding( const QString& subsystem_name , const QString& binding_key);
};

using BackendSubsystemsPtr = std::shared_ptr<BackendSubsystems>;


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
    std::future<QStringList> makeQuery(const QString& command, QStringList args);
    void redirectRequestAction(const QString& action , QStringList args);
public slots:
    virtual QStringList queryAbout(const QString& command, QStringList args) override;
};

using BackendPtr = std::shared_ptr<Backend>;

class BackendBuilder
{
private:
    uint                 threads_count_;
    BackendSubsystemsPtr subsystems_;
private:
    void resetSubsystems();
public:
    BackendBuilder():
        subsystems_{new BackendSubsystems}
    {}
    std::shared_ptr<Backend> build();
    BackendBuilder& addSubsystem(const QString& subsystem_name , IMediatorPtr subsystem);
    BackendBuilder& addSubsystemBinding(const QString& subsystem_name , const QString& binding_key);
    BackendBuilder& setThreadsCount(uint th_count);
};