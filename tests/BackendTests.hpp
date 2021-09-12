#pragma once

#include <gmock/gmock.h>

#include <forward_list>

#include "common_utils/WrappersList.hpp"
#include "common_utils/EventLoop.hpp"
#include "common_utils/Caller.hpp"



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


struct Utils
{
    WrappersList                wrappers_;
    DelayedEventLoop            event_loop_;
    ResultCheckerPtr            checker_;
    
    QueryAboutCallerFactory     query_caller_factory_;
    RequestActionCallerFactory  request_caller_factory_;
public:
    Utils():
        checker_{new ResultChecker} ,
        query_caller_factory_{checker_ , nullptr} ,
        request_caller_factory_{checker_ , nullptr} 
    {
        QObject::connect(&event_loop_ , &DelayedEventLoop::runned , 
                         &wrappers_ , &WrappersList::callAll);
        QObject::connect(&wrappers_ , &WrappersList::finished , 
                         &event_loop_ , &DelayedEventLoop::killTestEventLoop);
    }
public:
    void setBackend(BackendPtr backend)
    {
        query_caller_factory_.setBackend(backend);
        request_caller_factory_.setBackend(backend);
    }

};

class BackendTEST : public ::testing::Test
{
public:
    BackendTEST():
        query_loader_{"test_data/QueryAboutData.json"} ,
        action_loader_{"test_data/RequestActionData.json"}
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
    Utils                  utils_;
    BackendMediatorsMocks  mocks_;
    
    QueryAboutPackageLoader     query_loader_;
    RequestActionPackageLoader  action_loader_;

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
        return utils_.query_caller_factory_.produce(pack);
    }
    void appendRequestActionToList(RequestActionPackage pack , 
                                   MediatorMockPtr expected_target , 
                                   int times = 1)
    {
        appendFunctionWrapperToCallList(utils_.request_caller_factory_.produce(pack , expected_target) , 
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
    //special cases
    void simpleTest(std::string call_data ,  std::string precall_data , int calls_count = 1)
    {
        auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , 
                                             query_loader_.loadPackage(call_data) ,
                                             calls_count)};
        wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , 
                                                   query_loader_.loadPackage(precall_data) ,
                                                   calls_count));

        appendFunctionWrapperToCallList(std::move(wrapper) , calls_count);

        start();  
    }
};

