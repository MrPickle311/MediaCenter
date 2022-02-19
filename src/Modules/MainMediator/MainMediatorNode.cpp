#include "MainMediatorNode.hpp"

#include <functional>
#include <iostream>

namespace modules
{

using namespace std::placeholders;

QString MainMediatorMatcher::extractKey(QJsonDocument& command)
{
    std::cout << "Got it 1!\n";
    return "org.MediaCenter.DataStorage";
}

MainMediatorNode::MainMediatorNode() :
    matcher_{},
    med_{ std::make_shared<common::Mediator>(
        std::bind(&common::SystemNode::sendSignal, &node_, _1, _2),
        std::bind(&common::SystemNode::requestData, &node_, _1, _2),
        matcher_) },
    env_{ std::make_shared<common::SystemNodeEnvironment>() },
    node_{ "org.MediaCenter.MainMediator", env_, med_ }
{
    env_->addSystemNode("org.MediaCenter.DataStorage");
}

} // namespace modules
