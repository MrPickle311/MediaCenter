#pragma once
#include <QObject>
#include <SystemNode.hpp>

namespace modules
{

class AudioPageCommandsProcessor : public common::IBehaviourController
{
public:
    void onCoughtSignal(QJsonDocument message) override;
    QJsonDocument onRequestedData(QJsonDocument command) override;
};


class AudioPageConnector : public QObject
{
    Q_OBJECT;

public:
    explicit AudioPageConnector(QObject* parent);

private:
    common::SystemNode::BehaviourControllerType commands_processor_;
    common::SystemNode::NodeEnvironmentType env_;
    common::SystemNode node_;

public slots:
    void requestSongsPlaylist();
};

} // namespace modules
