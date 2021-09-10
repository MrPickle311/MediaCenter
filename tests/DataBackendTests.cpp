#include "DataBackendTests.hpp"
#include <QCoreApplication>

TEST(abc ,abcd)
{
    
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}