#include "QFunctionWrapper.hpp"

void QFunctionWrapper::invoke()
{
    func_body_();
    emit finished();
}

void QFunctionWrapper::setFunction(std::function<void ()> func)
{
    this->func_body_ = std::move(func);
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