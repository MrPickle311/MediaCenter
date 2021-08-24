#include "ThreadPoolTests.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

int func(int a , int b)
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}