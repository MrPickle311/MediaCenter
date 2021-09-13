#pragma once

#include "WrappersList.hpp"
#include "EventLoop.hpp"
#include "Caller.hpp"

class Factories
{
private:
    QueryAboutCallerFactory     query_caller_factory_;
    RequestActionCallerFactory  request_caller_factory_;
    QueryAboutPackageLoader     query_loader_;
    RequestActionPackageLoader  request_loader_;
public:
    Factories(ResultCheckerPtr checker,
              std::string queries_path , 
              std::string requests_path);
    void setMediator(IMediatorPtr mediator);

    RequestActionPackage    loadRequestPackage(std::string name);
    QueryAboutPackage       loadQueryPackage(std::string name);
    RequestActionCallerPtr  produceRequestCaller(const RequestActionPackage& pack , MediatorMockPtr mock);
    QueryAboutCallerPtr     produceQueryCaller(const QueryAboutPackage& pack);
};

struct TestsEngine
{
    WrappersList     wrappers_;
    DelayedEventLoop event_loop_;
    ResultCheckerPtr checker_;
public:
    TestsEngine();
};

class MediatorTester : public ::testing::Test
{
protected:
    TestsEngine  engine_;
    Factories factories_;

    IMediatorPtr                tested_mediator_;
public:
    virtual ~MediatorTester(){}
    MediatorTester(std::string queries_path , std::string requests_path);
public:
    void startEventLoop();
    void start();
    void appendFunctionToCallList(std::function<void()> function, int calls_count = 1);
    void appendFunctionWrapperToCallList(QFunctionWrapperPtr wrapper , int calls_count = 1);
    //tests setup
    QueryAboutCallerPtr createQueryAboutCaller(MediatorMock& target , 
                                               QueryAboutPackage pack , 
                                               int calls_count = 1);
    void appendRequestActionToList(RequestActionPackage pack , 
                                   MediatorMockPtr expected_target , 
                                   int calls_count = 1);
    //calls expectations
    void expectQueryAboutCall(MediatorMock& target ,
                              QueryAboutPackage call_package , 
                              int calls_count = 1 );
};