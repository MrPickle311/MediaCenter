#pragma once

#include <Backend.hpp>
#include <gmock/gmock.h>
#include <QTimer>
#include <future>
#include <QEventLoop>

class MediatorMOCK: public IMediator
{
    Q_OBJECT;
public:
    MOCK_METHOD(QVariant , queryAbout , (QString , QString) );
};


class UIMock : public QObject
{
    Q_OBJECT;
    std::shared_ptr<Backend> backend_;
public:
    virtual ~UIMock(){};
    UIMock(std::shared_ptr<Backend> backend, QObject *parent = nullptr);
signals:
    void requestAction(QString sender ,QString what , QVariantList args = {});
    void dataReady();
public slots:
    QVariant queryAbout(QString sender , QString what);
};

class DelayedEventLoop : public QObject
{
    Q_OBJECT
private:
    QEventLoop                loop_;
    std::chrono::milliseconds delay_;
private:
    void emitDelayedStartSignal();
public:
    DelayedEventLoop(int ms = 100);
    void startTestEventLoop();
public slots:
    void killTestEventLoop();
signals:
    void runned();
};

class FunctionWrapper : public QObject
{
    Q_OBJECT;
private:
    std::function<void()> func_;
public slots:
    void invoke();
public:
    void setFunction(std::function<void()> func);
};

class BackendTEST : public ::testing::Test
{
public:
    BackendTEST();
protected:

    //utils
    FunctionWrapper             wrapper_;
    DelayedEventLoop            loop_;
    UIMock                      ui_mock_;

    //tested obj
    std::shared_ptr<Backend>    backend_;

    //mocks
    MediatorMOCK                data_backend_mock_;
    MediatorMOCK                env_backend_mock_;
    MediatorMOCK                settings_backend_mock_;
protected:
    void initActionHandler();
};


// FRONTEND MOCK
