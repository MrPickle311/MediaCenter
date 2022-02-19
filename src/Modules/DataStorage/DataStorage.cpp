#include <DataStorageNode.hpp>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    modules::DataStorageNode node{&app};

    return QCoreApplication::exec();
}
