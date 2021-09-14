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

class Backend : public Mediator
{
    Q_OBJECT;
    friend class BackendBuilder;
private:
    Backend(uint threads_count);
private:
    TaskManager task_manager_;
private:
    virtual std::future<QStringList> makeQuery(const QString& command, const QStringList& args); 
public slots:
    virtual QStringList queryAbout(const QString& command, const QStringList& args) override;
};

using BackendPtr = std::shared_ptr<Backend>;

class BackendBuilder : public MediatorBuilder
{
private:
    uint  threads_count_;
public:
    BackendPtr buildBackend();
    BackendBuilder& setThreadsCount(uint th_count);
};