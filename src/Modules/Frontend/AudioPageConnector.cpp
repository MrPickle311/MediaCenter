#include "AudioPageConnector.hpp"

#include <iostream>

namespace modules
{

void AudioPageCommandsProcessor::onCoughtSignal(QJsonDocument message) {}

QJsonDocument AudioPageCommandsProcessor::onRequestedData(QJsonDocument command)
{
    return {};
}

AudioPageConnector::AudioPageConnector(QObject* parent) :
    QObject{ parent },
    commands_processor_{ std::make_shared<AudioPageCommandsProcessor>() },
    env_{ std::make_shared<common::SystemNodeEnvironment>() },
    node_{ "audio_connector", env_, commands_processor_ }
{
    env_->addSystemNode("org.MediaCenter.MainMediator");
}

void AudioPageConnector::requestSongsPlaylist()
{
    node_.sendSignal("org.MediaCenter.MainMediator", {});
}

} // namespace modules