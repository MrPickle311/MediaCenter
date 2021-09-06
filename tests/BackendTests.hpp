#pragma once

#include <Backend.hpp>
#include <gmock/gmock.h>

#include <CommonUtils.hpp>
#include <QSignalSpy>

#include <forward_list>
#include <atomic>

class MediatorMOCK: public IMediator
{
    Q_OBJECT;
public:
    MOCK_METHOD(QStringList , queryAbout , (QString , QStringList) );
};

class UIMock : public QObject
{
    Q_OBJECT;
    std::shared_ptr<Backend> backend_;
public:
    virtual ~UIMock(){};
    UIMock(QObject* parent = nullptr);
signals:
    void requestAction(QString command , QStringList args = {});
    void dataReady();
public slots:
    QStringList queryAbout(QString command , QStringList args = {});
public:
    void setBackend(std::shared_ptr<Backend> backend);
};

struct MediatorsMocks
{
    std::shared_ptr<MediatorMOCK>  data_storage_;
    std::shared_ptr<MediatorMOCK>  environment_;
    std::shared_ptr<MediatorMOCK>  settings_;
    MediatorsMocks():
        data_storage_{new MediatorMOCK},
        environment_{new MediatorMOCK},
        settings_{new MediatorMOCK}
    {}
};

class WrappersList : public QObject
{
    Q_OBJECT;
private:
    std::atomic_int tasks_finished_;
    QFunctionWrapperFactory factory_;
    std::list<std::shared_ptr<QFunctionWrapper>>  init_func_wrappers_;
private:
    void checkIfFinished()
    {
        if(tasks_finished_ == init_func_wrappers_.size())
        {
            emit finished();
            tasks_finished_ = 0;
        }
    }
    void connectWrapper(std::shared_ptr<QFunctionWrapper>& wrapper)
    {
        QObject::connect(this , &WrappersList::callAll ,
                         wrapper.get() , &QFunctionWrapper::invoke);

        QObject::connect(wrapper.get() , &QFunctionWrapper::finished ,
                         this , &WrappersList::updateCallState);
    }
    void pushBack(std::shared_ptr<QFunctionWrapper> wrapper)
    {
        init_func_wrappers_.push_back(std::move(wrapper));
    }
    void connectAndPush(std::shared_ptr<QFunctionWrapper> wrapper)
    {
        connectWrapper(wrapper);
        pushBack(std::move(wrapper));
    }
private slots:
    void updateCallState()
    {
        ++this->tasks_finished_;
        checkIfFinished();
    }
public:
    void append(std::function<void()> function)
    {
        factory_.setFunction(std::move(function));
        auto wrapper {factory_.produce()};

        connectAndPush(wrapper);
    }
    void append(std::shared_ptr<QFunctionWrapper> wrapper)
    {
        connectAndPush(wrapper);
    }
signals:
    void callAll();
    void finished();
};

struct Utils
{
    WrappersList      wrappers_;
    UIMock            ui_mock_;
    DelayedEventLoop  event_loop_;
public:
    Utils(std::shared_ptr<Backend> backend);
public:
    void setBackend(std::shared_ptr<Backend> backend);
};

class QueryAboutPackage
{
private:
    bool          is_empty_;
    QString       command_;
    QStringList   expected_result_;
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

    QString& command()
    {
        setPackageNotEmpty();
        return command_; 
    }
    const QString& command() const 
    { 
        return command_; 
    }

    QStringList& expectedResult()
    {
        setPackageNotEmpty();
        return expected_result_; 
    }
    const QStringList& result() const 
    { 
        return expected_result_; 
    }

    QStringList& callArguments()
    {
        setPackageNotEmpty();
        return call_args_; 
    }
    const QStringList& callArguments() const 
    { 
        return call_args_; 
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
    friend class QueryAboutMockFactory;
    Q_OBJECT;
private:
    std::shared_ptr<ResultChecker>  checker_;
    std::shared_ptr<Backend>        backend_;
    QueryAboutPackage               query_package_;
    std::shared_ptr<QueryAboutCaller> precall_;
private:
    void invokePrecall(QueryAboutPackage pack);
    void checkItself(QStringList result)
    {
        checker_->pattern() = query_package_.expectedResult();
        checker_->checkResult(result);
    }
private:
    QueryAboutCaller(std::shared_ptr<ResultChecker> checker,
                     std::shared_ptr<Backend>       backend , 
                     QueryAboutPackage              query_package):
            checker_{checker} ,
            backend_{backend} ,
            query_package_{std::move(query_package)}
    {
        std::function<void()> ui_action = [this]
        {
            //waits until searched QStringList is prepared
            auto result = backend_->queryAbout(query_package_.command() ,
                                               query_package_.callArguments());
                                  
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
    QueryAboutCaller& setPrecall(std::shared_ptr<QueryAboutCaller> precall)
    {
        this->precall_ = precall;
        return *this->precall_;
    }
};

class QueryAboutMockFactory
{
private:
    std::shared_ptr<ResultChecker>  checker_;
    std::shared_ptr<Backend>        backend_;
    // QueryAboutPackage               query_package_;
public:
    QueryAboutMockFactory(std::shared_ptr<ResultChecker>  checker ,
                          std::shared_ptr<Backend>        backend):
                checker_{checker} ,
                backend_{backend}
    {}
public:
    std::shared_ptr<QueryAboutCaller> produce(QueryAboutPackage query_package) const
    {
        return std::shared_ptr<QueryAboutCaller>{new QueryAboutCaller{ checker_ , 
                                                                       backend_ , 
                                                                       query_package
                                                                     }};
    }
    void setBackend(std::shared_ptr<Backend> backend)
    {
        this->backend_ = backend;
    }
};

class RequestActionCaller : public QFunctionWrapper
{

};

class BackendTEST : public ::testing::Test
{
public:
    BackendTEST();
    virtual ~BackendTEST(){}
protected:
    Utils           utils_;
    MediatorsMocks  mocks_;
    std::shared_ptr<ResultChecker> checker_;
    QueryAboutMockFactory factory_;

    //tested object
    std::shared_ptr<Backend>    backend_;
protected:
//tests setup
    void startEventLoop();
    void appendFunctionToCallList(std::function<void()> function, int count = 1)
    {
        for(int i{0}; i < count; ++i)
        {
            utils_.wrappers_.append(function);
        }
    }
    void appendFunctionWrapperToCallList(std::shared_ptr<QFunctionWrapper> wrapper , int times = 1)
    {
        for(int i{0}; i < times; ++i)
        {
            utils_.wrappers_.append(wrapper);
        }    
    }
    std::shared_ptr<QueryAboutCaller> createQueryAboutCaller(MediatorMOCK& target , 
                                                             QueryAboutPackage pack , 
                                                             int times = 1)
    {
        expectQueryAboutCall(target , pack , times);//WTF? times not here
        return factory_.produce(pack);
    }
    void start()
    {
        startEventLoop();
    }
//calls expectations
    void expectQueryAboutCall(MediatorMOCK& target ,
                              QueryAboutPackage call_package , 
                              int times = 1 )
    {
        using ::testing::Eq;
        using ::testing::Return;

        EXPECT_CALL(target, 
                    queryAbout(Eq(call_package.command()),
                               Eq(call_package.callArguments())))
            .Times(times)
            .WillRepeatedly(Return(call_package.result())
            );
    }
};

