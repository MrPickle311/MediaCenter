#include "MediatorTester.hpp"

Factories::Factories(ResultCheckerPtr checker,
                     std::string queries_path , 
                     std::string requests_path):
        query_caller_factory_{checker , nullptr} ,
        request_caller_factory_{checker , nullptr} ,
        query_loader_{queries_path} ,
        request_loader_{requests_path}
{}

void Factories::setMediator(IMediatorPtr mediator)
{
    query_caller_factory_.setMediator(mediator);
    request_caller_factory_.setMediator(mediator);
}

RequestActionPackage Factories::loadRequestPackage(std::string name) 
{
    return request_loader_.loadPackage(name);
}

QueryAboutPackage Factories::loadQueryPackage(std::string name) 
{
    return query_loader_.loadPackage(name);
}

RequestActionCallerPtr Factories::produceRequestCaller(const RequestActionPackage& pack , MediatorMockPtr mock) 
{
    return request_caller_factory_.produce(pack , mock );
}

QueryAboutCallerPtr Factories::produceQueryCaller(const QueryAboutPackage& pack) 
{
    return query_caller_factory_.produce(pack);
}

TestsEngine::TestsEngine():
    checker_{new ResultChecker}
{
    QObject::connect(&event_loop_ , &DelayedEventLoop::runned , 
                     &wrappers_ , &WrappersList::callAll);
    QObject::connect(&wrappers_ , &WrappersList::finished , 
                     &event_loop_ , &DelayedEventLoop::killTestEventLoop);
}

MediatorTester::MediatorTester(std::string queries_path , std::string requests_path): 
        factories_{engine_.checker_ , queries_path , requests_path }
{}

void MediatorTester::startEventLoop() 
{
    engine_.event_loop_.startTestEventLoop();
}

void MediatorTester::start()
{
    startEventLoop();
}

void MediatorTester::appendFunctionToCallList(std::function<void()> function, int calls_count)
{
    for(int i{0}; i < calls_count; ++i)
    {
        engine_.wrappers_.append(function);
    }
}

void MediatorTester::appendFunctionWrapperToCallList(QFunctionWrapperPtr wrapper , int calls_count)
{
    for(int i{0}; i < calls_count; ++i)
    {
        engine_.wrappers_.append(wrapper);
    }    
}

//tests setup
QueryAboutCallerPtr MediatorTester::createQueryAboutCaller(MediatorMock& target , 
                                                           QueryAboutPackage pack , 
                                                           int calls_count)
{
    expectQueryAboutCall(target , pack , calls_count);
    return factories_.produceQueryCaller(pack);
}

void MediatorTester::appendRequestActionToList(RequestActionPackage pack , 
                                               MediatorMockPtr expected_target , 
                                               int calls_count)
{
    appendFunctionWrapperToCallList(factories_.produceRequestCaller(pack , expected_target) , 
                                    calls_count );
}

//calls expectations
void MediatorTester::expectQueryAboutCall(MediatorMock& target ,
                                          QueryAboutPackage call_package , 
                                          int calls_count)
{
    using ::testing::Eq;
    using ::testing::Return;
    EXPECT_CALL(target, 
                queryAbout(Eq(call_package.getCommand()),
                           Eq(call_package.getCallArguments())))
        .Times(calls_count)
        .WillRepeatedly(Return(call_package.getExpectedResult()));
}