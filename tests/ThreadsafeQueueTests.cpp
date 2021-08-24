#include "ThreadsafeQueueTests.hpp"

TEST_F(QueueTests,SimpleSingleThreadTest)
{
    push(5,6,8,67);
    expectTryRefPopEqual(5);
    expectWaitRefPopEqual(6);
    expectWaitRefPopEqual(8);
    expectTryRefPopEqual(67);
    expectEmpty();
}

TEST_F(QueueTests, BorderTests)
{
    expectTryRefPopEqual(5,5);//no value change
    push(6);
    expectWaitPtrPopEqual(6);
    expectPopNull();
    clear();
    expectPopNull();
    expectEmpty();
}

TEST_F(QueueTests, ClearTests)
{
    clear();
    expectPopNull();
    expectPopNull();
    clear();
    expectPopNull();
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}