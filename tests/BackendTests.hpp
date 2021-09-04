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
    void checkIfFinished()
    {
        if(tasks_finished_ == init_func_wrappers_.size())
        {
            emit finished();
            tasks_finished_ = 0;
        }
    }
public:
    void append(std::function<void()> function)
    {
        factory_.setFunction(
            [function = std::move(function), this]
            {
                function();
                ++this->tasks_finished_;
                checkIfFinished();
            }
        );
        auto wrapper {factory_.produce()};

        QObject::connect(this , &WrappersList::callAll ,
                         wrapper.get(), &QFunctionWrapper::invoke);

        init_func_wrappers_.push_back(std::move(wrapper));
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

    QString& command()
    {
        setPackageNotEmpty();
        return command_; 
    }
    const QString& command() const 
    { 
        return command_; 
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

    ResultChecker checker_;
private:
    void invokePrecall(QueryAboutPackage pack);
protected:
//tests setup
    void startEventLoop();
    void appendInitialFunction(std::function<void()> function, int count = 1);
    void appendUIQueryAboutOnStart(QueryAboutPackage call_package , int count = 1);
//calls expectations
    void expectQueryAboutCall(MediatorMOCK& target ,
                              int count , 
                              QueryAboutPackage call_package ,
                              QueryAboutPackage pre_call_package = {});
    void expectSingleQueryAboutCall(MediatorMOCK& target ,
                                    QueryAboutPackage call_package ,
                                    QueryAboutPackage pre_call_package = {});
//testing methods
    void checkResult(const QStringList& result , const QueryAboutPackage& pattern);
    void testQueryAboutCall(QueryAboutPackage call_pack , int count = 1); 
};

