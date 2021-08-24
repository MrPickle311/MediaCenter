#include "ThreadPoolTests.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

template<typename T>
int add(T a , T b)
{
    return a + b;
}


TEST(TST, StabilityTest)
{
    using namespace boost::asio;
    using namespace boost;

    std::promise<int> promise;
    auto future = promise.get_future();

    thread_pool pool(4);
    post(pool, 
    [&]
    {
        promise.set_value(func(4,5));
    });

    int res {future.get()};

    //auto res {pool.submit(wrapper).get()};
    EXPECT_EQ(res,9);
}

TEST(TST, StabilityTest2)
{
    int threads_count{5};
    TaskManager manager{new ThreadSorageBoost{threads_count}};

    //no necessary 
    //auto future = flag.get_future();

    //internally invokes flag.get_future();
    int res = manager.addTask(add, 5 , 6).getFuture().get();

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
    int threads_count{5};
    TaskManager manager{new ThreadSorageBoost{threads_count}};

    std::promise<int> flag;

    uint task_count = 100;

    std::vector<std::future<int>> futures;
    futures.resize(task_count); 

    for(uint i{0}; i < task_count ; ++i)
    {
        futures[i] = manager.addTask(flag,add, 5 , 6).getFuture();
    }

    int sum{0};

    std::this_thread::sleep_for(std::chrono::milliseconds{400});

    for(uint i{0}; i < task_count ; ++i)
    {
        sum += futures[i].get();
    }

    EXPECT_EQ(sum,task_count);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}