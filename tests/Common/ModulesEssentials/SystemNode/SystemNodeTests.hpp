#pragma once

#include <EventLoop.hpp>
#include <SystemNode.hpp>
#include <WrappersList.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tests
{

class NodeHandleMock : public common::INodeHandle
{
public:
    MOCK_METHOD(void, sendSignal, (QByteArray message), (override));
    MOCK_METHOD(QByteArray, requestData, (QByteArray command), (override));
};

class SystemNodeEnvironmentMock : public common::ISystemNodeEnvironment
{
public:
    MOCK_METHOD(void, addSystemNode, (QString node_name), (override));
    MOCK_METHOD(common::INodeHandle&,
                getNodeHandle,
                (QString node_name),
                (override));
};

class BehaviourControllerMock : public common::IBehaviourController
{
public:
    MOCK_METHOD(void, onCoughtSignal, (QJsonDocument message), (override));
    MOCK_METHOD(QJsonDocument,
                onRequestedData,
                (QJsonDocument command),
                (override));
};

// class DBusTester : public common::NodeHandle
// {
//     Q_OBJECT;

// public:
//     DBusTester(QString tested_node_name);
// };

class SystemNodeTests : public ::testing::Test
{
public:
    SystemNodeTests();

protected:
    const QString test_node_name_{"org.media.testing"};

protected:
    std::shared_ptr<SystemNodeEnvironmentMock> env_mock_;
    std::shared_ptr<BehaviourControllerMock> behaviour_controller_mock_;
    common::SystemNode node_;

private:
    DelayedEventLoop loop_;
    WrappersList functions_;
    QFunctionWrapperFactory func_factory_;

protected:
    void addToExecutionList(std::function<void()> func)
    {
        func_factory_.setFunction(std::move(func));
        functions_.append(func_factory_.produce());
    }
    void start();
};
} // namespace tests