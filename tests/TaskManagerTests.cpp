#include "TaskManagerTests.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <functional>

int add(int a , int b)
{
    return a + b;
}

TEST(ThreadPoolTest, StabilityTest)
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

TEST(ThreadPoolTest, DutyTest)
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
    EXPECT_LE(elapsed_seconds.count(), 5.5);//exec time < 5.5 sec
}

void saveSth(std::shared_ptr<std::string> str)
{
    *str = "Result";
}

TEST(ThreadPoolTest , VoidTest)
{
    uint threads_count{3};
    TaskManager manager{threads_count};

    std::shared_ptr<std::string> str{new std::string};

    manager.addTask(saveSth,str).get();

    EXPECT_STREQ(str->c_str(),"Result");
}

TEST_F(ThreadPoolTests , QtEnvTest)
{
    std::string msg_to_test {"HappyMsg"};

    std::function<void()> invoked_function = 
    [&]()
    {
        manager_.addTask(
            [this, msg_to_test]
            { 
                this->received_msg_ = msg_to_test;
                loop_.killTestEventLoop();
            });
    };
    wrapper_.setFunction(std::move(invoked_function));
    loop_.startTestEventLoop();

    EXPECT_STREQ(this->received_msg_.c_str(), msg_to_test.c_str());
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}