#pragma once

#include <QObject>
#include <QVariant>
#include <QThreadPool>
#include <map>

class IMediator : public QObject
{
    Q_OBJECT
public:
    virtual ~IMediator(){}
    explicit IMediator(QObject *parent = nullptr);
public:
    virtual QVariant queryAbout(QString sender, QString what) = 0;
signals:
    void requestAction(QString sender,QString requestedAction,QVariantList args = {});
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
    Backend();
private:
    QThreadPool tasks_;

    //dependencies
    std::shared_ptr<IMediator> settings_backend_;
    std::shared_ptr<IMediator> data_backend_;
    std::shared_ptr<IMediator> environment_backend_;
public slots:
    virtual QVariant queryAbout(QString sender, QString what) override;
};
