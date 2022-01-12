#pragma once

#include <Mediator.hpp>
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

class MatcherMock : public ::testing::Test
{
public:
    MOCK_METHOD(QString, extractKey, (QJsonDocument&), (override));
};


class MediatorTests : public ::testing::Test
{
private:
protected:
    SystemNodeMock system_node_;
    MatcherMock matcher_;
    common::Mediator mediator_;

protected:
    MediatorTests();
};
} // namespace tests