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

class QueryAboutPackage
{
private:
    bool          is_empty_;
    std::string   sender_;
    std::string   what_;
    QStringList   result_;
    QStringList   call_args_;
private:
    void setPackageNotEmpty()
    {
        is_empty_ = false;
    }
public:
    QueryAboutPackage():
        is_empty_{true},
        call_args_{}
    {}
    operator bool() const
    {
        return !is_empty_;
    }

    std::string& sender()
    {
        setPackageNotEmpty();
        return sender_; 
    }
    const std::string& sender() const 
    { 
        return sender_; 
    }

    std::string& command()
    {
        setPackageNotEmpty();
        return what_; 
    }
    const std::string& command() const 
    { 
        return what_; 
    }

    QStringList& result()
    {
        setPackageNotEmpty();
        return result_; 
    }
    const QStringList& result() const 
    { 
        return result_; 
    }

    QStringList& callArguments()
    {
        setPackageNotEmpty();
        return result_; 
    }
    const QStringList& callArguments() const 
    { 
        return result_; 
    }
};

/*

#pragma region Interfaces

using QueryAboutDataPtr = std::shared_ptr<QueryAboutData>;

class IQueryAboutDataFactory
{
public:
    virtual QueryAboutDataPtr produce() const = 0;
};

class IQueryAboutMock
{
public:
    virtual void invoke() = 0;
    virtual QueryAboutDataPtr setNext(QueryAboutDataPtr next) = 0;
};

using QueryAboutMockPtr = std::shared_ptr<IQueryAboutMock>; 

using QueryAboutDataPtr = std::shared_ptr<IQueryAboutMock>;

class IQueryAboutMockFactory
{
public:
    virtual QueryAboutDataPtr produce(MediatorMOCK   target ,
                                      QueryAboutData query_data);
};

#pragma endregion Interfaces

#pragma region QueryAboutMock



class QueryAboutMock: public IQueryAboutMock
{
private:
    QueryAboutMockPtr next_;
    std::function<void()> body_;
private:
    QueryAboutMock(std::function<void()> body):
        next_{nullptr} , 
        body_{std::move(body)}
    {}
public:
    //COPY OR MOVE OPERATOR 
    virtual void invoke()
    {
        if(next_)
        {
            next_->invoke();
        }

        body_();
    }
    virtual QueryAboutDataPtr setNext(QueryAboutDataPtr next)
    {
        this->next_ = next;
    }
};

class QueryAboutMockFactory : public IQueryAboutMockFactory
{
private:
    std::shared_ptr<Backend>    backend_;
public:
    QueryAboutMockFactory(std::shared_ptr<Backend> backend):
        backend_{std::move(backend)}
    {

    }
    virtual QueryAboutDataPtr produce(MediatorMOCK   target ,
                                      QueryAboutData query_data)
    {//I finished HERE
        using ::testing::Eq;
    using ::testing::Return;

    EXPECT_CALL(target, queryAbout(Eq(sender),Eq(what),Eq(call_args)))
        .WillOnce(Return(result));
    }
};

#pragma endregion QueryAboutMock

*/

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
private:
    void invokePrecall(QueryAboutPackage pack);
protected:
//tests setup
    void startEventLoop();
    void setInitialFunction(std::function<void()> function);
    void setUIQueryAboutAsInit(QueryAboutPackage call_package);
//Result expectations
    void expectResultSize(int size);
    void expectResultElementEqualTo(int idx , std::string what);
//calls expectations
    void expectQueryAboutCall(MediatorMOCK& target ,
                              QueryAboutPackage call_package,
                              QueryAboutPackage pre_call_package = {});
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
    return backend_->queryAbout(sender,what,args);
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

void BackendTEST::setUIQueryAboutAsInit(QueryAboutPackage call_package) 
{
    std::function<void()> ui_action = [&]()
    {
        //waits until searched QStringList is prepared
        utils_.result = utils_.ui_mock_
                              .queryAbout(QString::fromStdString(call_package.sender()) ,
                                          QString::fromStdString(call_package.command()) ,
                                          call_package.callArguments());

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

void BackendTEST::invokePrecall(QueryAboutPackage pack)
{
    if(pack)
    {
        auto precall_result = backend_->queryAbout(pack.sender() ,
                                                   pack.command() ,
                                                   pack.callArguments());
        
        EXPECT_EQ(precall_result , pack.result());
    }
}

void BackendTEST::expectQueryAboutCall(MediatorMOCK& target ,
                                       QueryAboutPackage call_package,
                                       QueryAboutPackage pre_call_package) 
{
    using ::testing::Eq;

    EXPECT_CALL(target, 
                queryAbout(Eq(call_package.sender()),
                           Eq(call_package.command()),
                           Eq(call_package.callArguments())))
        .WillOnce(
            [&]
            {
                invokePrecall(pre_call_package);
                return call_package.result();
            }
        );
}

