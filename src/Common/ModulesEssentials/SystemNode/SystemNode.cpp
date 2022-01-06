#include "SystemNode.hpp"

namespace common
{

SystemNode::SystemNode(const QString& node_name, NodeEnvironmentType environment)
{
    registerThisNode(node_name);
}

void SystemNode::coughtSignal(QByteArray message) {}

QByteArray SystemNode::requestedData(QByteArray command) {}

void SystemNode::sendSignal(QString target_node, QByteArray message) {}

QByteArray SystemNode::requestData(QString name, QByteArray command) {}

// Void SystemNode::addSystemNode(QString name)
// {
//     if (!nodes_.contains(name))
//     {
//         nodes_.insert(name, createNewNodeHandle(name));
//         connectToNode(name);
//     }
// }

void SystemNode::registerThisNode(const QString& node_name)
{
    QDBusConnection::sessionBus().registerService(node_name);
}

// SystemNode::NodeHandle SystemNode::createNewNodeHandle(QString node_name)
// {
//     return std::make_unique<QDBusInterface>(
//         node_name, "/", "", QDBusConnection::sessionBus());
// }

} // namespace common