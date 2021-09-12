#include "Caller.hpp"

void ResultChecker::expectStringListsSizeEqual(const QStringList& first , const QStringList& second)
{
    ASSERT_EQ(first.size() , second.size());
}

void ResultChecker::expectStringsEqual(const QString& str1 , const QString& str2)
{
    EXPECT_STREQ(str1.toStdString().c_str() , 
                 str2.toStdString().c_str() );
}

void ResultChecker::expectStringListsEqual(const QStringList& first , const QStringList& second)
{
    expectStringListsSizeEqual(first , second);

    for(auto i{0}; i < first.size() ; ++i)
    {
        expectStringsEqual(first[i] , second[i]);
    }
}

Caller::Caller(ResultCheckerPtr   checker  ,
               BackendPtr         backend):
        checker_{checker},
        backend_{backend} 
{}

void QueryAboutCaller::checkItself(const QStringList& result)
{
    checker_->expectStringListsEqual(result , this->query_package_.getExpectedResult());
}

QueryAboutCaller::QueryAboutCaller(ResultCheckerPtr   checker,
                                   BackendPtr         backend , 
                                   QueryAboutPackage  query_package):
        Caller{checker , backend} ,
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

void QueryAboutCaller::tryInvokePrecall()
{
    if(this->precall_)
    {
        this->precall_->invoke();
    }
}

void QueryAboutCaller::invoke()
{
    tryInvokePrecall();
    QFunctionWrapper::invoke();
}

QueryAboutCaller& QueryAboutCaller::setPrecall(QueryAboutCallerPtr precall)
{
    this->precall_ = precall;
    return *this->precall_;
}

CallerFactory::CallerFactory(ResultCheckerPtr checker,
                             BackendPtr backend):
        checker_{checker} ,
        backend_{backend}
{}

void CallerFactory::setBackend(BackendPtr backend)
{
    this->backend_ = backend;
}

void CallerFactory::setChecker(ResultCheckerPtr checker) 
{
    this->checker_ = checker;
}

RequestActionCaller::RequestActionCaller(RequestActionPackage pack ,
                                         ResultCheckerPtr     checker,
                                         BackendPtr           backend , 
                                         MediatorMockPtr      expected_mock_to_call):
        Caller{checker , backend} ,
        pack_{pack} , 
        expected_mock_to_call_{expected_mock_to_call}
{
    QObject::connect(expected_mock_to_call_.get() , &MediatorMock::requestAction ,
        [this](auto command ,  auto args = {})
        {
            checker_->expectStringsEqual(command , pack_.getCommand());
            checker_->expectStringListsEqual(args , pack_.getCallArguments());
            
            emit finished();
        });
}

void RequestActionCaller::invoke()
{
    backend_->requestAction(pack_.getCommand() , pack_.getCallArguments());
}

QueryAboutCallerPtr QueryAboutCallerFactory::produce(QueryAboutPackage query_package) const
{
    return QueryAboutCallerPtr{new QueryAboutCaller{ checker_ , 
                                                     backend_ , 
                                                     query_package
                                                   }};
}

RequestActionCallerPtr RequestActionCallerFactory::produce(RequestActionPackage pack , 
                                                           MediatorMockPtr expected_mock_to_call) const
{
    return RequestActionCallerPtr{new RequestActionCaller{ 
                                                            pack ,
                                                            checker_ ,
                                                            backend_ , 
                                                            expected_mock_to_call
                                                         }};
}

