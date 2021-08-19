#pragma once

#include <QObject>
#include <QVariant>
#include <QThreadPool>
#include <map>

class IMediator : public QObject
{
    Q_OBJECT
public:
    explicit IMediator(QObject *parent = nullptr);
public slots:
    virtual void requestAction(QString sender,QString requestedAction,QVariantList args = {}) = 0;
    virtual QVariant queryAbout(QString sender, QString what) = 0;
signals:
    void requestUIAction(QString action);
};

class IProxy : public QObject
{
    Q_OBJECT
public:
    explicit IProxy(QObject *parent = nullptr);
public slots:
    virtual void requestAction(QString action , QVariantList args) = 0;
    virtual QStringList requestData(QString what) = 0;
};


// I have finished here
class Backend : public IMediator
{
private:
    std::map<QString, std::unique_ptr<IProxy>> modules_;
    QThreadPool tasks_;
public slots:
    virtual void requestAction(QString sender, QString requestedAction, QVariantList args) override;
    virtual QVariant queryAbout(QString sender, QString what) override;
};
