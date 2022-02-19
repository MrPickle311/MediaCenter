#include <MainMediatorNode.hpp>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    modules::MainMediatorNode main_mediator_node;

    return QCoreApplication::exec();
}
