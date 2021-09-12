#pragma once

#include <gtest/gtest.h>
#include <atomic>
#include <QStringList>
#include <memory>
#include <Backend.hpp>

#include "MediatorPackage.hpp"
#include "QFunctionWrapper.hpp"
#include "Mock.hpp"


class ResultChecker;
class QueryAboutCaller;
class RequestActionCaller;

using ResultCheckerPtr       = std::shared_ptr<ResultChecker>;
using QueryAboutCallerPtr    = std::shared_ptr<QueryAboutCaller>;
using RequestActionCallerPtr = std::shared_ptr<RequestActionCaller>;

class ResultChecker
{
private:
    void expectStringListsSizeEqual(const QStringList& first ,const QStringList& second );
public:
    void expectStringsEqual(const QString& first , const QString& second);
    void expectStringListsEqual(const QStringList& first , const QStringList& second);
};

class Caller : public QFunctionWrapper
{
protected:
    ResultCheckerPtr    checker_;
    BackendPtr          backend_;
public:
    Caller(ResultCheckerPtr   checker  ,
            BackendPtr         backend);
};

class QueryAboutCaller : public Caller
{
    friend class QueryAboutCallerFactory;
    Q_OBJECT;
private:
    QueryAboutPackage   query_package_;
    QueryAboutCallerPtr precall_;
private:
    void checkItself(const QStringList& result);
private:
    QueryAboutCaller(ResultCheckerPtr   checker,
                     BackendPtr         backend , 
                     QueryAboutPackage  query_package);
private:
    void tryInvokePrecall();
public:
    virtual void invoke();
    QueryAboutCaller& setPrecall(QueryAboutCallerPtr precall);
};

class RequestActionCaller : public Caller
{
    Q_OBJECT;
    friend class RequestActionCallerFactory;
private:
    RequestActionPackage    pack_;
    MediatorMockPtr         expected_mock_to_call_;
private:
    RequestActionCaller(RequestActionPackage pack ,
                        ResultCheckerPtr     checker,
                        BackendPtr           backend , 
                        MediatorMockPtr      expected_mock_to_call);
public:
    virtual void invoke();
};

class CallerFactory
{
protected:
    ResultCheckerPtr  checker_;
    BackendPtr        backend_;
public:
    CallerFactory(ResultCheckerPtr  checker,
                  BackendPtr backend);
public:
    void setBackend(BackendPtr backend); 
    void setChecker(ResultCheckerPtr checker);
};

class QueryAboutCallerFactory : public CallerFactory
{
public:
    using CallerFactory::CallerFactory;
    QueryAboutCallerPtr produce(QueryAboutPackage query_package) const;
};

class RequestActionCallerFactory : public CallerFactory
{
public:
    using CallerFactory::CallerFactory;
    RequestActionCallerPtr produce(RequestActionPackage pack , 
                                   MediatorMockPtr expected_mock_to_call) const;
};