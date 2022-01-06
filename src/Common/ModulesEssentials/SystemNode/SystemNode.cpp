#include "SystemNode.hpp"

namespace common
{

SystemNode::SystemNode(QString node_name) : node_name_{node_name}
{
    registerThisNode(node_name);
}

void SystemNode::coughtMessage(QByteArray message) {}

QByteArray SystemNode::requestedData(QByteArray command) {}

void SystemNode::sendMessage(QByteArray message) {}

QByteArray SystemNode::requestFrom(QString name, QByteArray command) {}

void SystemNode::addSystemNode(QString name)
{
    if (!nodes_.contains(name))
    {
        nodes_.insert(name, createNewNodeHandle(name));
        connectToNode(name);
    }
}

void SystemNode::registerThisNode(const QString& node_name)
{
    QDBusConnection::sessionBus().registerService(node_name);
}

SystemNode::NodeHandle SystemNode::createNewNodeHandle(QString node_name)
{
    return std::make_unique<QDBusInterface>(
        node_name, "/", "", QDBusConnection::sessionBus());
}

} // namespace common