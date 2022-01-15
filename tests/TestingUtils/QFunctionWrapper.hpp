#pragma once

#include <QObject>
#include <memory>

class IQFunctionWrapper : public QObject
{
    Q_OBJECT;
public:
    virtual void invoke() = 0;
    virtual void setFunction(std::function<void()> func) = 0;
signals:
    void sendSignal(std::string msg = {});
    void finished();
};

class QFunctionWrapper : public IQFunctionWrapper
{
    Q_OBJECT;
private:
    std::function<void()> func_body_;
public:
    virtual void invoke();
    virtual void setFunction(std::function<void()> func);
};

using QFunctionWrapperPtr = std::shared_ptr<QFunctionWrapper>;

class QFunctionWrapperFactory
{
private:
    std::function<void()> func_;
public:
    void setFunction(std::function<void()> func);
    std::shared_ptr<QFunctionWrapper> produce() const;
};