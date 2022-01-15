#include "WrappersList.hpp"

void WrappersList::checkIfFinished()
{
    if (tasks_finished_ == init_func_wrappers_.size())
    {
        emit finished();
        tasks_finished_ = 0;
    }
}
void WrappersList::connectWrapper(const QFunctionWrapperPtr& wrapper)
{
    connectToInvoke(wrapper);
    connectToNotifyAboutFinished(wrapper);
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

WrappersList::WrappersList(Qt::ConnectionType connection_type) :
    tasks_finished_{0}, connection_type_{connection_type}
{}

void WrappersList::append(std::function<void()> function)
{
    factory_.setFunction(std::move(function));
    auto wrapper{factory_.produce()};
    connectAndPush(wrapper);
}
void WrappersList::append(QFunctionWrapperPtr wrapper)
{
    connectAndPush(wrapper);
}
void WrappersList::connectToNotifyAboutFinished(
    const QFunctionWrapperPtr& wrapper)
{
    QObject::connect(wrapper.get(),
                     &QFunctionWrapper::finished,
                     this,
                     &WrappersList::updateCallState,
                     connection_type_);
}

void WrappersList::connectToInvoke(const QFunctionWrapperPtr& wrapper)
{

    QObject::connect(this,
                     &WrappersList::callAll,
                     wrapper.get(),
                     &QFunctionWrapper::invoke,
                     connection_type_);
}
