#pragma once

#include <SystemNode.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class SystemNodeTests : public ::testing::Test
{

protected:
    common::SystemNode node_;

    SystemNodeEnvironmentMock env_mock_;
};