#pragma once

#include <QObject>
#include <map>
#include <filesystem>

//MEDIATOR TOO!!!!

class IEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit IEnvironment(QObject *parent = nullptr);
public slots:
    virtual void addIconPath(std::string path , std::string object_id) = 0;
    virtual QString getIconPath(QString object) = 0;
};

class Environment : public IEnvironment
{
    Q_OBJECT;
private:
    std::map<std::string , std::filesystem::path> frontend_icons_;
    std::filesystem::path parent_path_;
private:
    bool isParentPathSet() const;
    void addRelativeIconPath(std::string relative_path , std::string object_id);
    void addNormalIconPath(std::string relative_path , std::string object_id);
    decltype (auto) getIconInserter() const;
public:
    void setParentPath(const std::filesystem::path &parent_path);
public slots:
    virtual void addIconPath(std::string path , std::string object_id) override;
    virtual QString getIconPath(QString object_id) override;
};
