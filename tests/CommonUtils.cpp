#include <CommonUtils.hpp>
#include <iostream>
void DelayedEventLoop::emitDelayedStartSignal()
{
    std::this_thread::sleep_for(this->delay_);
    emit runned();
}

DelayedEventLoop::DelayedEventLoop(int ms):
    delay_{ms}
{}

void DelayedEventLoop::startTestEventLoop()
{
    std::thread th{&DelayedEventLoop::emitDelayedStartSignal,this};
    loop_.exec();
    th.join();
}

void DelayedEventLoop::killTestEventLoop()
{
    loop_.quit();
}

void QFunctionWrapper::invoke()
{
    func_();
}

void QFunctionWrapper::setFunction(std::function<void ()> func)
{
    this->func_ = std::move(func);
}

void QFunctionWrapperFactory::setFunction(std::function<void()> func) 
{
    this->func_ = std::move(func);
}

std::shared_ptr<QFunctionWrapper> QFunctionWrapperFactory::produce() const
{
    std::shared_ptr<QFunctionWrapper> wrapper{new QFunctionWrapper};
    wrapper->setFunction(this->func_);

    return wrapper;
}