#pragma once

#include <filesystem>

#include "JsonPackage.hpp"

class MediatorPackage 
{
protected:
    bool          is_empty_;
protected:
    void setPackageNotEmpty();
public:
    MediatorPackage();
    operator bool() const;
};

class CallablePackage : 
    public JsonPackage ,
    public MediatorPackage
{
public:
    CallablePackage();

    void setCommand(QString cmd);
    const QString& getCommand() const;

    void setCallArguments(const QStringList& args);
    const QStringList& getCallArguments() const;
};

class QueryAboutPackage : public CallablePackage
{
    friend class QueryAboutPackageFactory;
public:
    QueryAboutPackage();

    void setExpectedResult(const QStringList& res);
    const QStringList& getExpectedResult() const;
};

using RequestActionPackage = CallablePackage;

class JsonFile
{
private:
    QJsonDocument    doc_;
    QFile            json_file_;
private:
    void printError(std::string msg) const;
    void tryOpenJsonFile();
    void checkIfFileExists() const;
public:
    explicit JsonFile(QString path = {});
    QJsonObject tryLoadJsonObject();
    void setPath(QString path);
};

class JsonPackageLoader
{
protected:
    QJsonObject      entire_json_object_;
    JsonFile         file_;

    PackageExtractor extractor_;
    PackageChecker   checker_;
protected:
    QJsonObject readPackage(std::string pack_name) const;

    QJsonObject tryLoadLocalPackage(std::string pack_name) const;
    void        tryExtractResult(const QJsonObject& obj , JsonPackage& pack);
    void        tryExtract(JsonPackage& result , std::string pack_name);
public:
    JsonPackageLoader(std::string relative_path_to_json);
};

class QueryAboutPackageLoader : public JsonPackageLoader
{
public:
    using JsonPackageLoader::JsonPackageLoader;
    QueryAboutPackage loadPackage(std::string pack_name);
};

class RequestActionPackageLoader : public JsonPackageLoader
{
public:
    using JsonPackageLoader::JsonPackageLoader;
    RequestActionPackage loadPackage(std::string pack_name);  
};