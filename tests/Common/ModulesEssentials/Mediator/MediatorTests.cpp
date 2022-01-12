#include "MediatorTests.hpp"

#include <QCoreApplication>

namespace tests
{

using namespace std::placeholders;

MediatorTests::MediatorTests() :
    mediator_{ std::bind(&SystemNodeMock::sendSignal, &system_node_, _1, _2),
               std::bind(&SystemNodeMock::requestData, &system_node_, _1, _2),
               matcher_ }
{}


} // namespace tests


int main(int argc, char* argv[])
{
    QCoreApplication app{ argc, argv };
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}