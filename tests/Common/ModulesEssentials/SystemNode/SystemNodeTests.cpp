#include "SystemNodeTests.hpp"

#include <QCoreApplication>

TEST_F(SystemNodeTests, LocalUnitTest)
{

    constexpr auto node_name_1{"org.App.Node1"};
    constexpr auto node_name_2{"org.App.Node2"};

    EXPECT_CALL(mock, addSystemNode(node_name_1)).WillOnce();

    EXPECT_CALL(mock, addSystemNode(node_name_2)).WillOnce();

    EXPECT_CALL(mock, getNodeHandle(node_name_1))
        .WillOnce(NodeHandle{node_name_1});

    EXPECT_CALL(mock, getNodeHandle(node_name_2))
        .WillOnce(NodeHandle{node_name_2});

    addToExecutionList([this] {
        this->env_mock_.addSystemNode(node_name_1);
        this->env_mock_.addSystemNode(node_name_2);
    });

    addToExecutionList([this] {
        this->node_.sendSignal(node_name_1, QByteArray{});
        this->node_.sendSignal(node_name_2, QByteArray{});
    });

    start();
}

int main(int argc, char* argv[])
{
    QCoreApplication app{argc, argv};
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}