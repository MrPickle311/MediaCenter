#include "DataStorageNode.hpp"

#include <iostream>

namespace modules
{

void DataStorageCommandsProcessor::onCoughtSignal(QJsonDocument message)
{
    std::cout << "Got it 2!\n";
}

QJsonDocument
DataStorageCommandsProcessor::onRequestedData(QJsonDocument command)
{
    return QJsonDocument();
}


DataStorageNode::DataStorageNode(QObject* parent) :
    QObject{ parent },
    commands_processor_{ std::make_shared<DataStorageCommandsProcessor>() },
    env_{ std::make_shared<common::SystemNodeEnvironment>() },
    node_{ "org.MediaCenter.DataStorage", env_, commands_processor_ }
{}
} // namespace modules
