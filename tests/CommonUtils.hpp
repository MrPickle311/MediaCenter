#pragma once

#include <QTimer>
#include <future>
#include <QEventLoop>
#include <QCoreApplication>

class DelayedEventLoop : public QObject
{
    Q_OBJECT
private:
    QEventLoop                loop_;
    std::chrono::milliseconds delay_;
private:
    void emitDelayedStartSignal();
public:
    DelayedEventLoop(int ms = 100);
    void startTestEventLoop();
public slots:
    void killTestEventLoop();
signals:
    void runned();
};

class QFunctionWrapper : public QObject
{
    Q_OBJECT;
private:
    std::function<void()> func_;
public slots:
    void invoke();
public:
    void setFunction(std::function<void()> func);
signals:
    void sendSignal(std::string msg = {});
};