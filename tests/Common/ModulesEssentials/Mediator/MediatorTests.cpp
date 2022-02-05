#include "MediatorTests.hpp"

namespace tests
{

using namespace std::placeholders;

MediatorTests::MediatorTests() :
    mediator_{ std::bind(&SystemNodeMock::sendSignal, &system_node_, _1, _2),
               std::bind(&SystemNodeMock::requestData, &system_node_, _1, _2),
               matcher_ }
{}

void MediatorTests::addCodeToRun(SimpleSequentialCodeRunner::FunctionType&& func)
{
    runner_.addCodeToRun(std::move(func));
}

void MediatorTests::runAll()
{
    runner_.runAll();
}

void MediatorTests::simulateSystemNodeSignalCought(const QJsonDocument& message)
{
    addCodeToRun([message, this]() {
        mediator_.onCoughtSignal(message);
    });
}

void MediatorTests::simulateSystemNodeRequestedData(const QJsonDocument& message)
{
    addCodeToRun([message, this]() {
        mediator_.onRequestedData(message);
    });
}


TEST_F(MediatorTests, InternalCallingTest)
{
    QJsonDocument json;
    QString target;

    EXPECT_CALL(this->matcher_, extractKey(json)).Times(2);

    EXPECT_CALL(this->system_node_, sendSignal(target, json)).Times(1);
    EXPECT_CALL(this->system_node_, requestData(target, json)).Times(1);

    simulateSystemNodeRequestedData(json);
    simulateSystemNodeSignalCought(json);

    runAll();
}


} // namespace tests


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}