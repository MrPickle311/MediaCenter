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
    void requestAction(QString sender ,QString what , QStringList args = {});
    void dataReady();
public slots:
    QStringList queryAbout(QString sender , QString what , QStringList args = {});
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
    QString       sender_;
    QString       what_;
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

    QString& sender()
    {
        setPackageNotEmpty();
        return sender_; 
    }
    const QString& sender() const 
    { 
        return sender_; 
    }

    QString& command()
    {
        setPackageNotEmpty();
        return what_; 
    }
    const QString& command() const 
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
    void expectResultElementEqualTo(int idx , QString what);
//calls expectations
    void expectQueryAboutCall(MediatorMOCK& target ,
                              QueryAboutPackage call_package,
                              QueryAboutPackage pre_call_package = {});
};

