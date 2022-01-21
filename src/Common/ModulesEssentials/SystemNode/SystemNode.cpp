#include "SystemNode.hpp"

#include <QDBusReply>
#include <iostream>

namespace common
{

const QString NodeHandle::coughtSignal_{ "coughtSignal" };
const QString NodeHandle::requestedData_{ "requestedData" };
const QString NodeHandle::no_interface_{ "" };

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
    iface_{ createNewNodeHandle(node_name, default_path) }, name_{ node_name }
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

QString NodeHandle::getNodeName() const
{
    return name_;
}

void SystemNodeEnvironment::addSystemNode(QString node_name)
{
    nodes_.insert(
        std::make_pair(node_name, std::make_shared<NodeHandle>(node_name)));
}

INodeHandle& SystemNodeEnvironment::getNodeHandle(QString node_name)
{
    return *nodes_.at(node_name);
}

uint SystemNodeEnvironment::getNodeHandlesCount() const
{
    return nodes_.size();
}


SystemNode::SystemNode(const QString& node_name,
                       NodeEnvironmentType environment,
                       BehaviourControllerType behaviour__controller) :
    node_name_{ node_name },
    env_{ environment },
    behaviour__controller_{ behaviour__controller }
{
    registerThisNode(node_name);
    makeNodeAvailable();
}

void SystemNode::coughtSignal(QByteArray message)
{
    QJsonDocument doc{ QJsonDocument::fromJson(message) };

    behaviour__controller_->onCoughtSignal(std::move(doc));
}

QByteArray SystemNode::requestedData(QByteArray command)
{
    QJsonDocument doc{ QJsonDocument::fromJson(command) };

    return behaviour__controller_->onRequestedData(std::move(doc)).toJson();
}

void SystemNode::sendSignal(QString target_node, QJsonDocument message)
{
    auto bytes{ message.toJson() };
    env_->getNodeHandle(target_node).sendSignal(std::move(bytes));
}

QJsonDocument SystemNode::requestData(QString target_node, QJsonDocument command)
{
    auto bytes{ command.toJson() };
    return QJsonDocument::fromJson(
        env_->getNodeHandle(target_node).requestData(std::move(bytes)));
}

void SystemNode::registerThisNode(const QString& node_name)
{
    QDBusConnection::sessionBus().registerService(node_name);
}

void SystemNode::makeNodeAvailable()
{
    auto state = QDBusConnection::sessionBus().registerObject(
        "/", this, QDBusConnection::ExportAllSlots);
}
} // namespace common