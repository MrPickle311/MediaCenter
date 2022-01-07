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
    node_{"org.Test", env_mock_, behaviour_controller_mock_}
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

// TEST(NodeHandleTests, ConnectionTest)
// {

// }


TEST_F(SystemNodeTests, LocalUnitTest)
{
    QString node_name_1{"org.App.Node1"};
    QString node_name_2{"org.App.Node2"};

    common::NodeHandle handle_1{node_name_1};
    common::NodeHandle handle_2{node_name_2};

    EXPECT_CALL(*env_mock_, addSystemNode(QString{node_name_1})).Times(1);

    EXPECT_CALL(*env_mock_, addSystemNode(QString{node_name_2})).Times(1);

    EXPECT_CALL(*env_mock_, getNodeHandle(node_name_1))
        .WillOnce(ReturnRef(handle_1));

    EXPECT_CALL(*env_mock_, getNodeHandle(node_name_2))
        .WillOnce(ReturnRef(handle_2));

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
} // namespace tests

int main(int argc, char* argv[])
{
    QCoreApplication app{argc, argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}