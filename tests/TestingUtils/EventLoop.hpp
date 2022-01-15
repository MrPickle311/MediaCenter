#pragma once

#include <QEventLoop>
#include <QObject>
#include <future>

class IEventLoop : public QObject
{
    Q_OBJECT;

public:
    virtual void startTestEventLoop() = 0;
public slots:
    virtual void killTestEventLoop() = 0;
signals:
    void runned();
};

class DelayedEventLoop : public IEventLoop
{
    Q_OBJECT;

public:
    using TimeType = std::chrono::milliseconds;

private:
    QEventLoop loop_;
    TimeType delay_;

public:
    DelayedEventLoop(TimeType delay = TimeType{100});
    virtual void startTestEventLoop();
public slots:
    virtual void killTestEventLoop();
};