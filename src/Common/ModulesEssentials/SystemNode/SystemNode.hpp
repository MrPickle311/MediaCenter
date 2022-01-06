#pragma once

#include <QDBusConnection>
#include <QDBusInterface>
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
    virtual void addSystemNode(QString node_name) = 0;
    virtual INodeHandle& getNodeHandle(QString node_name) = 0;
};

class ISystemNode : public QObject
{
    Q_OBJECT;
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
    friend class SystemNodeBuilder;

    using NodeEnvironmentType = std::unique_ptr<ISystemNodeEnvironment>;

private:
    QString node_name_;

    NodeEnvironmentType env_;

private:
    void registerThisNode(const QString& node_name);

public:
    SystemNode(const QString& node_name, NodeEnvironmentType environment);
protected slots:
    DBusIncomingSignal virtual void coughtSignal(QByteArray message);
    QByteArray virtual requestedData(QByteArray command);
public slots:
    virtual void sendSignal(QString target_node, QByteArray message);
    virtual QByteArray requestData(QString name, QByteArray command);
};

} // namespace common