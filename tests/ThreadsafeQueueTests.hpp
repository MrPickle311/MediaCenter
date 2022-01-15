#pragma once

#include <gtest/gtest.h>
#include <ThreadsafeQueue.hpp>

class QueueTests: public ::testing::Test
{
protected:
    ThreadsafeQueue<int> queue;
    template<typename ... Args>

    //utils
    void push(Args ... args)
    {
        (queue.push(args), ...);
    }

    //expectations
    void expectTryRefPopEqual(int val, int init_val = 0)
    {
        int temp{init_val};
        EXPECT_NO_THROW(queue.tryPop(temp));
        EXPECT_EQ(temp , val);
    }
    void expectTryPtrPopEqual(int val)
    {
        auto result {queue.tryPop()};
        ASSERT_NE(result,nullptr);
        EXPECT_EQ(*result , val);
    }
    void expectWaitRefPopEqual(int val, int init_val = 0)
    {
        int temp{init_val};
        EXPECT_NO_THROW(queue.tryPop(temp));
        EXPECT_EQ(temp , val);
    }
    void expectWaitPtrPopEqual(int val)
    {
        auto result {queue.waitAndPop()};
        ASSERT_NE(result,nullptr);
        EXPECT_EQ(*result , val);
    }
    void expectEmpty()
    {
        EXPECT_TRUE(queue.empty());
    }
    void expectPopNull()
    {
        EXPECT_TRUE(queue.tryPop() == nullptr);
    }
    void clear()
    {
        EXPECT_NO_THROW(queue.clear());
    }
};