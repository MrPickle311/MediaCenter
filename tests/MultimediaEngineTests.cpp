#include "MultimediaEngineTests.hpp"
#include <QCoreApplication>

TEST_F(MulitmediaEngineTests , LogicTests)
{
    
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}