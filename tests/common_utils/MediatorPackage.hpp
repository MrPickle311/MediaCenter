#pragma once

#include <filesystem>

#include "JsonPackage.hpp"

class QueryAboutPackage : public JsonPackage
{
    friend class QueryAboutPackageFactory;
private:
    bool          is_empty_;
private:
    void setPackageNotEmpty();
public:
    QueryAboutPackage();

    operator bool() const;

    void setCommand(QString cmd);
    const QString& getCommand() const;

    void setExpectedResult(const QStringList& res);
    const QStringList& getExpectedResult() const;

    void setCallArguments(const QStringList& args);
    const QStringList& getCallArguments() const;
};

class QueryAboutPackageLoader
{
private:
    QJsonDocument    doc_;
    QFile            json_file_;
    QJsonObject      entire_json_object_;

    PackageExtractor extractor_;
    PackageChecker   checker_;
private:
    void printError(std::string msg) const;
    void tryOpenJsonFile();
    void tryLoadJsonFile();
    void checkIfFileExists() const;
    QJsonObject readPackage(std::string pack_name) const;
public:
    QueryAboutPackageLoader(std::string relative_path_to_json);
    QueryAboutPackage loadPackage(std::string pack_name);
};