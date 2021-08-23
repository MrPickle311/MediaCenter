#include "ThreadsafeQueueTests.hpp"
#include <QCoreApplication>

TEST(TEST,SimpleSingleThreadTest)
{
    ThreadsafeQueue<int> queue;
    queue.push(5);
    queue.push(6);
    queue.push(8);
    queue.push(67);
    int a;
    queue.tryPop(a);
    EXPECT_EQ(a,5);
    queue.waitAndPop(a);
    EXPECT_EQ(a,6);
    queue.waitAndPop(a);
    EXPECT_EQ(a,8);
    queue.tryPop(a);
    EXPECT_EQ(a,67);
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}