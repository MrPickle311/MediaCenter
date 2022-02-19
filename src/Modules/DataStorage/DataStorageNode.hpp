#pragma once
#include <SystemNode.hpp>

namespace modules
{


class DataStorageCommandsProcessor : public common::IBehaviourController
{
public:
    void onCoughtSignal(QJsonDocument message) override;
    QJsonDocument onRequestedData(QJsonDocument command) override;
};


class DataStorageNode : public QObject
{
Q_OBJECT;

public:
    DataStorageNode(QObject* parent);

private:
    common::SystemNode::BehaviourControllerType commands_processor_;
    common::SystemNode::NodeEnvironmentType env_;
    common::SystemNode node_;

public slots:
    void requestSongsPlaylist() {}
};

}