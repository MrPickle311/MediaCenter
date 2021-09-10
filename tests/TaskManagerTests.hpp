#pragma once

#include <gmock/gmock.h>
#include <TaskManager.hpp>

#include "common_utils/EventLoop.hpp"
#include "common_utils/QFunctionWrapper.hpp"

// @brief Testing class , automatic number of threads 
class ThreadPoolTests : public ::testing::Test
{
protected:
    TaskManager manager_;
    DelayedEventLoop loop_;
    QFunctionWrapper wrapper_;
    std::string received_msg_;
protected:
    ThreadPoolTests()
    {
        QObject::connect(&loop_,&DelayedEventLoop::runned, &wrapper_ , &QFunctionWrapper::invoke);
    }
};

