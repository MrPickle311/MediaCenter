#include "SystemNodeTests.hpp"

#include <QCoreApplication>
#include <QObject>

namespace tests
{

using ::testing::Return;
using ::testing::ReturnRef;

SystemNodeTests::SystemNodeTests() :
    env_mock_{std::make_shared<SystemNodeEnvironmentMock>()},
    behaviour_controller_mock_{std::make_shared<BehaviourControllerMock>()},
    functions_{Qt::QueuedConnection},
    node_{test_node_name_, env_mock_, behaviour_controller_mock_}
{
    QObject::connect(
        &loop_, &DelayedEventLoop::runned, &functions_, &WrappersList::callAll);

    QObject::connect(&functions_,
                     &WrappersList::finished,
                     &loop_,
                     &DelayedEventLoop::killTestEventLoop);
}

void SystemNodeTests::start()
{
    loop_.startTestEventLoop();
}

TEST_F(SystemNodeTests, LocalTest)
{
    QString node_name_1{"org.App.Node1"};
    QString node_name_2{"org.App.Node2"};

    NodeHandleMock handle_mock;

    EXPECT_CALL(*env_mock_, addSystemNode(QString{node_name_1})).Times(1);

    EXPECT_CALL(*env_mock_, addSystemNode(QString{node_name_2})).Times(1);

    EXPECT_CALL(*env_mock_, getNodeHandle(node_name_1))
        .WillOnce(ReturnRef(handle_mock));

    EXPECT_CALL(*env_mock_, getNodeHandle(node_name_2))
        .WillOnce(ReturnRef(handle_mock));


    addToExecutionList([=, this] {
        this->env_mock_->addSystemNode(node_name_1);
        this->env_mock_->addSystemNode(node_name_2);
    });

    addToExecutionList([=, this] {
        this->node_.sendSignal(node_name_1, QByteArray{});
        this->node_.sendSignal(node_name_2, QByteArray{});
    });

    start();
}

TEST_F(SystemNodeTests, SelfRemoteTest)
{
    common::NodeHandle handle{test_node_name_};

    QByteArray message{"abcd"};
    QByteArray return_value{"efgh"};
    QJsonDocument json{};

    EXPECT_CALL(*behaviour_controller_mock_, onCoughtSignal(json)).Times(1);

    EXPECT_CALL(*behaviour_controller_mock_, onRequestedData(json))
        .WillOnce(Return(json));

    addToExecutionList([&] {
        handle.requestData(message);
    });

    addToExecutionList([&] {
        handle.sendSignal(message);
    });

    start();
}

} // namespace tests

int main(int argc, char* argv[])
{
    QCoreApplication app{argc, argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}