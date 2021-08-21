#pragma once

#include <QObject>
#include <map>
#include <filesystem>

class IEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit IEnvironment(QObject *parent = nullptr);
public slots:
    virtual QString getIconPath(QString object) = 0;
};

class Environment : public IEnvironment
{
    Q_OBJECT;
private:
    std::map<std::string , std::filesystem::path> frontend_icons_;
private:

public slots:
    virtual QString getIconPath(QString object) override;
};
