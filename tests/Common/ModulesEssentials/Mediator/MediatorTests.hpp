#pragma once

#include <Mediator.hpp>
#include <SimpleSequentialCodeRunner.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tests
{

class SystemNodeMock : public common::ISystemNode
{
public:
    MOCK_METHOD(void, coughtSignal, (QByteArray), (override));
    MOCK_METHOD(QByteArray, requestedData, (QByteArray), (override));
    MOCK_METHOD(void, sendSignal, (QString, QJsonDocument), (override));
    MOCK_METHOD(QJsonDocument,
                requestData,
                (QString, QJsonDocument),
                (override));
};

class MatcherMock : public common::IMatcher
{
public:
    MOCK_METHOD(QString, extractKey, (QJsonDocument&), (override));
};


class MediatorTests : public ::testing::Test
{
private:
    SimpleSequentialCodeRunner runner_;

protected:
    SystemNodeMock system_node_;
    MatcherMock matcher_;
    common::Mediator mediator_;

protected:
    MediatorTests();

private:
    void addCodeToRun(SimpleSequentialCodeRunner::FunctionType&& func);

protected:
    void runAll();

    void simulateSystemNodeSignalCought(QJsonDocument message);
    void simulateSystemNodeRequestedData(QJsonDocument message);
};
} // namespace tests