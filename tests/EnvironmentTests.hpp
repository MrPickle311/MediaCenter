#pragma once

#include <gtest/gtest.h>
#include <Environment.hpp>

class EnvironmentTests : public ::testing::Test
{
protected:
    Environment env_;
protected:
    EnvironmentTests();
};

