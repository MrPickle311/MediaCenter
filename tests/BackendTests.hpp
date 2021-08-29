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
    UIMock(std::shared_ptr<Backend> backend, QObject* parent = nullptr);
signals:
    void requestAction(QString sender ,QString what , QVariantList args = {});
    void dataReady();
public slots:
    QStringList queryAbout(QString sender , QString what , QStringList args = {});
};

struct MediatorsMocks
{
    MediatorMOCK  data_storage_;
    MediatorMOCK  environment_;
    MediatorMOCK  settings_;
};

struct Utils
{
    UIMock            ui_mock_;
    QFunctionWrapper  initizal_function_wrapper_;
    DelayedEventLoop  event_loop_;
    QStringList       result;
public:
    Utils(std::shared_ptr<Backend> backend);
};

class BackendTEST : public ::testing::Test
{
public:
    BackendTEST();
    virtual ~BackendTEST(){}
protected:
    Utils           utils_;
    MediatorsMocks  mocks_;
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
    void expectResultSize(int size);
    void expectResultElementEqualTo(int idx,std::string what);
//calls expectations
    void expectQueryAboutCall(MediatorMOCK& target ,
                              std::string   sender ,
                              std::string   what ,
                              QStringList   result , 
                              QStringList   call_args = {});
};

struct QueryAboutData
{
    //make getters and setters
    std::string   sender;
    std::string   what;
    QStringList   result;
    QStringList   call_args;
};

class IQueryAboutDataFactory
{
public:
    virtual QueryAboutData produce() const = 0;
};

class IQueryAboutInvokation
{
public:

};

class IQueryAboutMockFactory
{
public:

};

Utils::Utils(std::shared_ptr<Backend> backend):
        ui_mock_(std::move(backend))
{
    QObject::connect(&event_loop_ , &DelayedEventLoop::runned , 
                     &initizal_function_wrapper_ , &QFunctionWrapper::invoke);
    QObject::connect(&ui_mock_ , &UIMock::dataReady , 
                     &event_loop_ , &DelayedEventLoop::killTestEventLoop);
}

UIMock::UIMock(std::shared_ptr<Backend> backend, QObject *parent) :
    QObject(parent),
    backend_(backend)
{
    QObject::connect(this , &UIMock::requestAction , backend_.get() , &Backend::requestAction );
}

QStringList UIMock::queryAbout(QString sender, QString what,  QStringList args)
{
    emit dataReady();
    return backend_->queryAbout(sender,what);
}


BackendTEST::BackendTEST():
    utils_{backend_},
    backend_{nullptr}
{
    BackendBuilder builder;

    builder.setDataBackendDependency(&mocks_.data_storage_);
    builder.setEnvironmentDependency(&mocks_.environment_);
    builder.setSettingsDependency(&mocks_.settings_);
    builder.setMaxThreadsCount(3);

    backend_ = std::move(builder.getBackendObject());
}

void BackendTEST::startEventLoop() 
{
    utils_.event_loop_.startTestEventLoop();
}

void BackendTEST::setInitialFunction(std::function<void()> function) 
{
    utils_.initizal_function_wrapper_.setFunction(std::move(function));
}

void BackendTEST::setUIQueryAboutAsInit(std::string  sender ,
                                        std::string  what   ,
                                        QStringList  call_args) 
{
    std::function<void()> ui_action = [&]()
    {
        //waits until searched QStringList is prepared
        //how about settings mock ?
        //the point is that settings mock returns value immediately,
        //so i have to introduce another mock pre-call here

        // mediator query struct and any factory pattern ?
        // function factory!!!
        // expectQueryAboutCall factory !!!
        // std::function factory !!!
        // responsibility chain template !!!

        utils_.result = utils_.ui_mock_.queryAbout(QString::fromStdString(sender) ,
                                                   QString::fromStdString(what) ,
                                                   call_args);
        emit utils_.ui_mock_.dataReady();
    };
 
    setInitialFunction(ui_action);
}

void BackendTEST::expectResultSize(int size) 
{
    EXPECT_EQ(utils_.result.size(),size);
}

void BackendTEST::expectResultElementEqualTo(int idx,std::string what) 
{
    EXPECT_STREQ(utils_.result.at(idx).toStdString().c_str(),what.c_str());
}

void BackendTEST::expectQueryAboutCall(MediatorMOCK& target ,
                                       std::string   sender ,
                                       std::string   what ,
                                       QStringList   result , 
                                       QStringList   call_args) 
{
    using ::testing::Eq;
    using ::testing::Return;

    EXPECT_CALL(target, queryAbout(Eq(sender),Eq(what),Eq(call_args)))
        .WillOnce(
            [&]
            {
                if(additional_initial_query)
                    additional_initial_query(DATA);
                return result;
            });
}

