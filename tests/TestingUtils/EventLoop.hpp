#pragma once

#include <QObject>
#include <QEventLoop>
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
private:
    QEventLoop                loop_;
    std::chrono::milliseconds delay_;
private:
    void emitDelayedStartSignal();
public:
    DelayedEventLoop(int ms = 100);
    virtual void startTestEventLoop();
public slots:
    virtual void killTestEventLoop();
};