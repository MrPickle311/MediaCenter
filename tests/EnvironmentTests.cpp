#include "EnvironmentTests.hpp"

EnvironmentTests::EnvironmentTests()
{}

TEST_F(EnvironmentTests , PathTests)
{
    env_.addIconPath("data/add.svg", "Add");
    env_.addIconPath("data/clear.svg", "Clear");
    env_.addIconPath("data/close_icon.svg", "CloseIcon");

    EXPECT_STREQ(env_.getIconPath("Add").toStdString().c_str(),"qrc:/data/add.svg");
    EXPECT_STREQ(env_.getIconPath("Clear").toStdString().c_str(),"qrc:/data/clear.svg");
    EXPECT_STREQ(env_.getIconPath("CloseIcon").toStdString().c_str(),"qrc:/data/close_icon.svg");
}

TEST_F(EnvironmentTests , RelativePathTests)
{
    env_.setParentPath("data");

    env_.addIconPath("add.svg", "Add");
    env_.addIconPath("clear.svg", "Clear");
    env_.addIconPath("close_icon.svg", "CloseIcon");

    EXPECT_STREQ(env_.getIconPath("Add").toStdString().c_str(),"qrc:/data/add.svg");
    EXPECT_STREQ(env_.getIconPath("Clear").toStdString().c_str(),"qrc:/data/clear.svg");
    EXPECT_STREQ(env_.getIconPath("CloseIcon").toStdString().c_str(),"qrc:/data/close_icon.svg");
}
