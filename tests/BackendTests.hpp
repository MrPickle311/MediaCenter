#pragma once

#include <Backend.hpp>
#include <gmock/gmock.h>

#include <CommonUtils.hpp>
#include <QSignalSpy>

class MediatorMOCK: public IMediator
{
    Q_OBJECT;
public:
    MOCK_METHOD(QStringList , queryAbout , (QString , QString, QStringList) );

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
    QStringList queryAbout(QString sender , QString what , QStringList);
};

class BactendTestObjectsWrapper
{
protected:

};

struct MediatorsMocks
{
    MediatorMOCK  data_storage_;
    MediatorMOCK  environment_;
    MediatorMOCK  settings_;
};

struct Utils
{
    QFunctionWrapper  initizal_function_wrapper_;
    DelayedEventLoop  event_loop_;
    UIMock            ui_mock_;
    QStringList       result;

};

class BackendTEST : public ::testing::Test
{
public:
    BackendTEST();
protected:
    Utils utils_;
    MediatorsMocks mocks_;
    //tested obj
    std::shared_ptr<Backend>    backend_;
protected:
//tests setup
    void startEventLoop();
    void setInitialFunction(std::function<void()> function);
    void setUIQueryAboutAsInit(std::string  sender ,
                               std::string  what   ,
                               QStringList  call_args = {});
//Result expectations
    void expectCanConvertToStringList();
    void expectResultSize(int size);
    void expectResultElementEqualTo(int idx,std::string what);
//calls expectations
    void expectQueryAboutCall(MediatorMOCK& target ,
                              std::string   sender ,
                              std::string   what ,
                              QStringList   result , 
                              QStringList   call_args = {});
};


// FRONTEND MOCK
