#include "SystemNode.hpp"

#include <QDBusReply>
#include <iostream>

namespace common
{

const QString NodeHandle::coughtSignal_{"coughtSignal"};
const QString NodeHandle::requestedData_{"requestedData"};
const QString NodeHandle::no_interface_{""};

NodeHandle::NodeHandleInternalType
NodeHandle::createNewNodeHandle(QString node_name, QString path)
{
    std::unique_ptr<QDBusInterface> temp;
    try
    {
        temp = std::make_unique<QDBusInterface>(
            node_name, path, no_interface_, QDBusConnection::sessionBus());
    }
    catch (const std::exception& e)
    {
        if (!temp)
        {
            std::cout << "Nullptr!\n";
        }
        std::cout << "Error : " << e.what() << '\n';
        throw;
    }


    return temp;
}

NodeHandle::NodeHandle(QString node_name, QString default_path) :
    iface_{createNewNodeHandle(node_name, default_path)}
{}

void NodeHandle::sendSignal(QByteArray command)
{
    iface_->asyncCall(coughtSignal_, command);
}

QByteArray NodeHandle::requestData(QByteArray command)
{
    QDBusReply<QByteArray> result;
    try
    {
        result = iface_->call(requestedData_, command);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

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
    makeNodeAvailable();
}

void SystemNode::coughtSignal(QByteArray message)
{
    QJsonDocument doc{QJsonDocument::fromJson(message)};

    behaviour__controller_->onCoughtSignal(doc);
}

QByteArray SystemNode::requestedData(QByteArray command)
{
    QJsonDocument doc{QJsonDocument::fromJson(command)};

    return behaviour__controller_->onRequestedData(doc).toJson();
}

void SystemNode::sendSignal(QString target_node, QByteArray message)
{
    env_->getNodeHandle(target_node).sendSignal(std::move(message));
}

QByteArray SystemNode::requestData(QString target_node, QByteArray command)
{
    return env_->getNodeHandle(target_node).requestData(command);
}

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
    auto state = QDBusConnection::sessionBus().registerService(node_name);
}

void SystemNode::makeNodeAvailable()
{
    auto state = QDBusConnection::sessionBus().registerObject(
        "/", this, QDBusConnection::ExportAllSlots);
}
} // namespace common