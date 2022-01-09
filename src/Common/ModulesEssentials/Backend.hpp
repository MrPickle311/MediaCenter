#pragma once

#include "Mediator/Matcher.hpp"
#include "Mediator/Mediator.hpp"
#include "TaskManager.hpp"

#include <QObject>
#include <map>

class Backend : public Mediator
{
    Q_OBJECT;
    friend class BackendBuilder;

public:
    Backend(uint threads_count);

private:
    TaskManager task_manager_;

private:
    virtual std::future<QStringList> makeQuery(const QString& command,
                                               const QStringList& args);
public slots:
    virtual QStringList queryAbout(const QString& command,
                                   const QStringList& args) override;
};

using BackendPtr = std::shared_ptr<Backend>;

class BackendBuilder : public MediatorBuilder
{
private:
    uint threads_count_;

public:
    BackendPtr buildBackend();
    BackendBuilder& setThreadsCount(uint th_count);
};