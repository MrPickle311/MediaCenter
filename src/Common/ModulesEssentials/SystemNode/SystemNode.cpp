#include "SystemNode.hpp"

#include <QDBusReply>

namespace common
{

const QString NodeHandle::coughtSignal_{"coughtSignal"};
const QString NodeHandle::requestedData_{"requestedData"};

NodeHandle::NodeHandleInternalType
NodeHandle::createNewNodeHandle(QString node_name)
{
    return std::make_unique<QDBusInterface>(
        node_name, "/", "", QDBusConnection::sessionBus());
}

NodeHandle::NodeHandle(QString node_name) :
    iface_{createNewNodeHandle(node_name)}
{}

void NodeHandle::sendSignal(QByteArray command)
{
    iface_->asyncCall(coughtSignal_, command);
}

QByteArray NodeHandle::requestData(QByteArray command)
{
    QDBusReply<QByteArray> result = iface_->call(requestedData_, command);
    return result;
}


SystemNode::SystemNode(const QString& node_name,
                       NodeEnvironmentType environment,
                       BehaviourControllerType behaviour__controller) :
    node_name_{node_name},
    env_{environment},
    behaviour__controller_{behaviour__controller}
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

} // namespace common