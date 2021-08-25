#include "ThreadPoolTests.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <functional>

int add(int a , int b)
{
    return a + b;
}

TEST(TST, StabilityTest)
{
    uint threads_count{5};
    TaskManager manager{threads_count};

    auto res = manager.addTask(add,5,6).get();

    EXPECT_EQ(res,11);
}

//just a load
int aggravatingFunction()
{
    std::this_thread::sleep_for(std::chrono::milliseconds{500});
    return 1;
}

TEST(TST, DutyTest)
{
    uint threads_count{10};
    TaskManager manager{threads_count};

    uint task_count = 100;

    std::vector<std::future<int>> futures;
    futures.resize(task_count); 

    for(uint i{0}; i < task_count ; ++i)
    {
        futures[i] = manager.addTask(aggravatingFunction);
    }

    uint sum{0};

    std::this_thread::sleep_for(std::chrono::milliseconds{400});
    
    auto start = std::chrono::steady_clock::now();

    for(uint i{0}; i < task_count ; ++i)
    {
        sum += futures[i].get();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    EXPECT_EQ(sum,task_count);
    EXPECT_NEAR(elapsed_seconds.count(),5.0,0.5);
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}