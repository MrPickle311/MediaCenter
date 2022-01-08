#pragma once

#include <QDBusConnection>
#include <QDBusInterface>
#include <QJsonDocument>
#include <QJsonObject>
#include <map>

#define DBusIncomingSignal Q_NOREPLY

namespace common
{

class INodeHandle
{
public:
    virtual ~INodeHandle() = default;
    virtual void sendSignal(QByteArray message) = 0;
    virtual QByteArray requestData(QByteArray command) = 0;
    virtual QString getNodeName() const = 0;
};

class ISystemNodeEnvironment
{
public:
    virtual ~ISystemNodeEnvironment() = default;
    virtual void addSystemNode(QString node_name) = 0;
    virtual INodeHandle& getNodeHandle(QString node_name) = 0;
    virtual uint getNodeHandlesCount() const = 0;
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
    static const QString coughtSignal_;
    static const QString requestedData_;
    static const QString no_interface_;

private:
    NodeHandleInternalType iface_;
    QString name_;

private:
    NodeHandleInternalType createNewNodeHandle(QString node_name, QString path);

public:
    NodeHandle(QString node_name, QString default_path = {"/"});

public:
    virtual void sendSignal(QByteArray message);
    virtual QByteArray requestData(QByteArray command);
    virtual QString getNodeName() const;
};


class SystemNodeEnvironment : public ISystemNodeEnvironment
{
private:
    std::map<QString, std::shared_ptr<INodeHandle>> nodes_;

public:
    virtual void addSystemNode(QString node_name);
    virtual INodeHandle& getNodeHandle(QString node_name);
    virtual uint getNodeHandlesCount() const;
};

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
    void makeNodeAvailable();

public:
    SystemNode(const QString& node_name,
               NodeEnvironmentType environment,
               BehaviourControllerType behaviour__controller);
public slots:
    virtual void coughtSignal(QByteArray message);
    virtual QByteArray requestedData(QByteArray command);
public slots:
    virtual void sendSignal(QString target_node, QByteArray message);
    virtual QByteArray requestData(QString target_node, QByteArray command);
};

} // namespace common