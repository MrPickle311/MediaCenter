#pragma once

#include <QObject>
#include <QVariant>
#include <QThreadPool>
#include <map>
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

class Backend : public IMediator
{
    Q_OBJECT;
    friend class BackendBuilder;
private:
    Backend(uint threads_count);
private:
    TaskManager task_manager_;

    //dependencies
    std::shared_ptr<IMediator> settings_backend_;
    std::shared_ptr<IMediator> data_backend_;
    std::shared_ptr<IMediator> environment_backend_;

    std::map<QString , std::shared_ptr<IMediator>&> bindings_;
private:
    void addBinding(QString key , std::shared_ptr<IMediator>& target);
    QString getTargetKey(QString command);
    std::shared_ptr<IMediator>& redirect(QString command);
    std::future<QStringList> makeQuery(QString command, QStringList args);
public slots:
    virtual QStringList queryAbout(QString command, QStringList args) override;
};

class BackendBuilder
{
private:
    uint threads_count_;
    std::shared_ptr<IMediator> data_backend_;
    std::shared_ptr<IMediator> env_backend_;
    std::shared_ptr<IMediator> settings_backend_;
public:
    std::shared_ptr<Backend> build();
    BackendBuilder& setDataBackendDependency(std::shared_ptr<IMediator> data_backend);
    BackendBuilder& setEnvironmentDependency(std::shared_ptr<IMediator> env_backend);
    BackendBuilder& setSettingsDependency(std::shared_ptr<IMediator> settings_backend);
    BackendBuilder& setThreadsCount(uint th_count);
};