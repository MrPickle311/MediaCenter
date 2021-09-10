#pragma once

#include <Backend.hpp>
#include <gmock/gmock.h>

#include <forward_list>
#include <atomic>

#include "common_utils/Mock.hpp"
#include "common_utils/QFunctionWrapper.hpp"
#include "common_utils/WrappersList.hpp"
#include "common_utils/EventLoop.hpp"
#include "common_utils/MediatorPackage.hpp"

class ResultChecker;
class QueryAboutCaller;
class RequestActionCaller;

using ResultCheckerPtr       = std::shared_ptr<ResultChecker>;
using QueryAboutCallerPtr    = std::shared_ptr<QueryAboutCaller>;
using RequestActionCallerPtr = std::shared_ptr<RequestActionCaller>;

struct BackendMediatorsMocks
{
    MediatorMockPtr  data_storage_;
    MediatorMockPtr  environment_;
    MediatorMockPtr  settings_;
    BackendMediatorsMocks():
        data_storage_{new MediatorMock},
        environment_{new MediatorMock},
        settings_{new MediatorMock}
    {}
};

class ResultChecker
{
private:
    QStringList pattern_;
private:
    void expectResultSizeEqualToPattern(QStringList result)
    {
        ASSERT_EQ(result.size() , pattern_.size());
    }
    void expectResultElementEqualTo(QStringList result , int idx)
    {
        EXPECT_STREQ(pattern_.at(idx).toStdString().c_str() , 
                     result.at(idx).toStdString().c_str());
    }
    void expectResultPatternMatching(QStringList result)
    {
        for(int i{0}; i < pattern_.size() ; ++i)
        {
            expectResultElementEqualTo(result , i);
        }
    }
public:
    QStringList& pattern()
    {
        return pattern_; 
    }
    const QStringList& pattern() const 
    { 
        return pattern_; 
    }
    void checkResult(QStringList result)
    {
        expectResultSizeEqualToPattern(result);
        expectResultPatternMatching(result);
    }
};

class QueryAboutCaller : public QFunctionWrapper
{
    friend class QueryAboutCallerFactory;
    Q_OBJECT;
private:
    ResultCheckerPtr    checker_;
    BackendPtr          backend_;
    QueryAboutPackage   query_package_;
    QueryAboutCallerPtr precall_;
private:
    void invokePrecall(QueryAboutPackage pack);
    void checkItself(QStringList result)
    {
        checker_->pattern() = query_package_.getExpectedResult();
        checker_->checkResult(result);
    }
private:
    QueryAboutCaller(ResultCheckerPtr   checker,
                     BackendPtr         backend , 
                     QueryAboutPackage  query_package):
            checker_{checker} ,
            backend_{backend} ,
            query_package_{std::move(query_package)}
    {
        std::function<void()> ui_action = [this]
        {
            //waits until searched QStringList is prepared
            auto result = backend_->queryAbout(query_package_.getCommand() ,
                                               query_package_.getCallArguments());
                                  
            checkItself(result);
        };

        QFunctionWrapper::setFunction(std::move(ui_action));
    }
private:
    void tryInvokePrecall()
    {
        if(this->precall_)
        {
            this->precall_->invoke();
        }
    }
public:
    virtual void invoke()
    {
        tryInvokePrecall();
        QFunctionWrapper::invoke();
    }
    QueryAboutCaller& setPrecall(QueryAboutCallerPtr precall)
    {
        this->precall_ = precall;
        return *this->precall_;
    }
};

class QueryAboutCallerFactory
{
private:
    ResultCheckerPtr  checker_;
    BackendPtr        backend_;
public:
    QueryAboutCallerFactory(ResultCheckerPtr  checker ,
                          BackendPtr          backend):
                checker_{checker} ,
                backend_{backend}
    {}
public:
    QueryAboutCallerPtr produce(QueryAboutPackage query_package) const
    {
        return QueryAboutCallerPtr{new QueryAboutCaller{ checker_ , 
                                                         backend_ , 
                                                         query_package
                                                       }};
    }
    void setBackend(BackendPtr backend)
    {
        this->backend_ = backend;
    }
};

class RequestActionCaller : public QFunctionWrapper
{
    Q_OBJECT;
    friend class RequestActionCallerFactory;
private:
    QString           command_;
    BackendPtr        backend_;
    MediatorMockPtr   expected_mock_to_call_;
private:
    RequestActionCaller(QString command , 
                        BackendPtr backend , 
                        MediatorMockPtr   expected_mock_to_call):
                command_{command} , 
                backend_{backend} ,
                expected_mock_to_call_{expected_mock_to_call}
    {
        QObject::connect(expected_mock_to_call_.get() , &MediatorMock::requestAction ,
            [this](auto requestedAction , auto args = {})
            {
                EXPECT_STREQ(requestedAction.toStdString().c_str() , 
                             command_.toStdString().c_str() );
                emit finished();
            });
    }
public:
    virtual void invoke()
    {
        backend_->requestAction(command_);
    }
};

class RequestActionCallerFactory
{
private:
    BackendPtr  backend_;
public:
    RequestActionCallerFactory(BackendPtr backend):
            backend_{backend}
    {}
public:
    RequestActionCallerPtr produce(QString command , MediatorMockPtr expected_mock_to_call) const
    {
        return RequestActionCallerPtr{new RequestActionCaller{ 
                                                                command , 
                                                                backend_ , 
                                                                expected_mock_to_call
                                                             }};
    }
    void setBackend(BackendPtr backend)
    {
        this->backend_ = backend;
    }
};

struct Utils
{
    WrappersList                wrappers_;
    DelayedEventLoop            event_loop_;
    ResultCheckerPtr            checker_;
    QueryAboutCallerFactory     query_factory_;
    RequestActionCallerFactory  request_factory_;
public:
    Utils(BackendPtr backend):
        checker_{new ResultChecker} ,
        query_factory_{checker_ , nullptr} ,
        request_factory_{nullptr}
    {
        QObject::connect(&event_loop_ , &DelayedEventLoop::runned , 
                         &wrappers_ , &WrappersList::callAll);
        QObject::connect(&wrappers_ , &WrappersList::finished , 
                         &event_loop_ , &DelayedEventLoop::killTestEventLoop);
    }
public:
    void setBackend(BackendPtr backend)
    {
        query_factory_.setBackend(backend);
        request_factory_.setBackend(backend);
    }

};

class BackendTEST : public ::testing::Test
{
public:
    BackendTEST():
        utils_{backend_},
        backend_{nullptr}
    {
        BackendBuilder builder;

        builder.addSubsystem("DataStorage" , mocks_.data_storage_)
               .addSubsystem("Environment" , mocks_.environment_)
               .addSubsystem("Settings" , mocks_.settings_);

        builder.addSubsystemBinding("DataStorage" , "Search")
               .addSubsystemBinding("DataStorage" , "Playlist")
               .addSubsystemBinding("Settings"    , "Mediapaths")
               .addSubsystemBinding("Settings"    , "Appdir")
               .setThreadsCount(10);

        backend_ = builder.build();

        // expect that backend is initilized
        EXPECT_NE(backend_.get() , nullptr);

        // set dependency
        utils_.setBackend(backend_);
    }
    virtual ~BackendTEST(){}
protected:
    Utils           utils_;
    BackendMediatorsMocks  mocks_;
    
    //tested object
    BackendPtr    backend_;
protected:
//tests setup
    void startEventLoop() 
    {
        utils_.event_loop_.startTestEventLoop();
    }
    void appendFunctionToCallList(std::function<void()> function, int times = 1)
    {
        for(int i{0}; i < times; ++i)
        {
            utils_.wrappers_.append(function);
        }
    }
    void appendFunctionWrapperToCallList(QFunctionWrapperPtr wrapper , int times = 1)
    {
        for(int i{0}; i < times; ++i)
        {
            utils_.wrappers_.append(wrapper);
        }    
    }
    QueryAboutCallerPtr createQueryAboutCaller(MediatorMock& target , 
                                               QueryAboutPackage pack , 
                                               int times = 1)
    {
        expectQueryAboutCall(target , pack , times);
        return utils_.query_factory_.produce(pack);
    }
    void appendRequestActionToList(QString command , 
                                   MediatorMockPtr expected_target , 
                                   int times = 1)
    {
        appendFunctionWrapperToCallList(utils_.request_factory_.produce(command , expected_target) , 
                                        times );
    }
    void start()
    {
        startEventLoop();
    }
//calls expectations
    void expectQueryAboutCall(MediatorMock& target ,
                              QueryAboutPackage call_package , 
                              int times = 1 )
    {
        using ::testing::Eq;
        using ::testing::Return;

        EXPECT_CALL(target, 
                    queryAbout(Eq(call_package.getCommand()),
                               Eq(call_package.getCallArguments())))
            .Times(times)
            .WillRepeatedly(Return(call_package.getExpectedResult())
            );
    }
};

