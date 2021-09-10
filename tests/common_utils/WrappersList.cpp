#include "WrappersList.hpp"

void WrappersList::checkIfFinished()
{
    if(tasks_finished_ == init_func_wrappers_.size())
    {
        emit finished();
        tasks_finished_ = 0;
    }
}
void WrappersList::connectWrapper(const QFunctionWrapperPtr& wrapper)
{
    QObject::connect(this , &WrappersList::callAll ,
                     wrapper.get() , &QFunctionWrapper::invoke);
    QObject::connect(wrapper.get() , &QFunctionWrapper::finished ,
                     this , &WrappersList::updateCallState);
}
void WrappersList::pushBack(QFunctionWrapperPtr wrapper)
{
    init_func_wrappers_.push_back(std::move(wrapper));
}
void WrappersList::connectAndPush(QFunctionWrapperPtr wrapper)
{
    connectWrapper(wrapper);
    pushBack(std::move(wrapper));
}

void WrappersList::updateCallState()
{
    ++this->tasks_finished_;
    checkIfFinished();
}

void WrappersList::append(std::function<void()> function)
{
    factory_.setFunction(std::move(function));
    auto wrapper {factory_.produce()};
    connectAndPush(wrapper);
}
void WrappersList::append(QFunctionWrapperPtr wrapper)
{
    connectAndPush(wrapper);
}