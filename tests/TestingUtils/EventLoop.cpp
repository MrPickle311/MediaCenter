#include "EventLoop.hpp"

#include <QTimer>
#include <thread>

DelayedEventLoop::DelayedEventLoop(TimeType delay) : delay_{delay} {}

void DelayedEventLoop::startTestEventLoop()
{
    QTimer::singleShot(delay_, this, &DelayedEventLoop::runned);
    loop_.exec();
}

void DelayedEventLoop::killTestEventLoop()
{
    loop_.quit();
}