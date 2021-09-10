#pragma once

#include "QFunctionWrapper.hpp"

class IWrappersList : public QObject
{
    Q_OBJECT;
public:
    virtual void append(std::function<void()> function) = 0;
    virtual void append(QFunctionWrapperPtr wrapper) = 0;
signals:
    void callAll();
    void finished();
};

class WrappersList : public IWrappersList
{
    Q_OBJECT;
private:
    std::atomic_int                 tasks_finished_;
    QFunctionWrapperFactory         factory_;
    std::list<QFunctionWrapperPtr>  init_func_wrappers_;
private:
    void checkIfFinished();
    void connectWrapper(const QFunctionWrapperPtr& wrapper);
    void pushBack(QFunctionWrapperPtr wrapper);
    void connectAndPush(QFunctionWrapperPtr wrapper);
private slots:
    void updateCallState();
public:
    virtual void append(std::function<void()> function);
    virtual void append(QFunctionWrapperPtr wrapper);
};
