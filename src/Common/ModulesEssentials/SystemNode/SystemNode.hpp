#pragma once

#include <QDBusConnection>
#include <QDBusInterface>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

#define DBusIncomingSignal Q_NOREPLY

namespace common
{

class INodeHandle
{
public:
    virtual void sendSignal(QByteArray message) = 0;
    virtual QByteArray requestData(QByteArray command) = 0;
};

class ISystemNodeEnvironment
{
public:
    virtual ~ISystemNodeEnvironment() = default;
    virtual void addSystemNode(QString node_name) = 0;
    virtual INodeHandle& getNodeHandle(QString node_name) = 0;
};

class IBehaviourController
{
public:
    virtual ~IBehaviourController() = default;
    virtual void onCoughtSignal(QJsonDocument message) = 0;
    virtual QJsonDocument onRequestedData(QJsonDocument command) = 0;
};

class ISystemNode : public QObject
{
    Q_OBJECT;

public:
    ~ISystemNode() = default;
protected slots:
    virtual DBusIncomingSignal void coughtSignal(QByteArray message) = 0;

    virtual QByteArray requestedData(QByteArray command) = 0;
public slots:
    virtual void sendSignal(QString target_node_node, QByteArray message) = 0;
    virtual QByteArray requestData(QString target_node_name,
                                   QByteArray command) = 0;
};


class NodeHandle : public INodeHandle
{
    using NodeHandleInternalType = std::unique_ptr<QDBusInterface>;

private:
    NodeHandleInternalType iface_;
    static const QString coughtSignal_;
    static const QString requestedData_;

private:
    NodeHandleInternalType createNewNodeHandle(QString node_name);

public:
    NodeHandle(QString node_name);

public:
    virtual void sendSignal(QByteArray message);
    virtual QByteArray requestData(QByteArray command);
};


// class SystemNodeEnvironment : public ISystemNodeEnvironment
// {
// private:
//     QMap<QString, INodeHandle> nodes_;

// private:
//     void connectToNode(const QString& node_name);
// };

class SystemNode : public ISystemNode
{
    Q_OBJECT;
    friend class SystemNodeBuilder;

    using NodeEnvironmentType = std::shared_ptr<ISystemNodeEnvironment>;
    using BehaviourControllerType = std::shared_ptr<IBehaviourController>;

private:
    QString node_name_;

    NodeEnvironmentType env_;
    BehaviourControllerType behaviour__controller_;

private:
    void registerThisNode(const QString& node_name);

public:
    SystemNode(const QString& node_name,
               NodeEnvironmentType environment,
               BehaviourControllerType behaviour__controller);
protected slots:
    virtual void coughtSignal(QByteArray message);
    virtual QByteArray requestedData(QByteArray command);
public slots:
    virtual void sendSignal(QString target_node, QByteArray message);
    virtual QByteArray requestData(QString name, QByteArray command);
};

} // namespace common