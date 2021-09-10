#pragma once

#include <QString>
#include <QStringList>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include <filesystem>

class QueryAboutPackage
{
    friend class QueryAboutPackageFactory;
private:
    bool          is_empty_;
    QString       command_;
    QStringList   expected_result_;
    QStringList   call_args_;
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
    QJsonDocument       doc_;
    QFile               json_file_;
    QJsonObject         loaded_object_;
private:
    QJsonObject readPackage(std::string pack_name) const;
    bool containsPackage(std::string pack_name) const;
    void printError(std::string msg);
    void checkIfFileExists() const;
    void tryOpenJsonFile();
    void tryLoadJsonFile();
    bool isJsonPackValid(const QJsonObject& obj) const;
    QueryAboutPackage extractQueryPackage(const QJsonObject& obj) const;
public:
    QueryAboutPackageLoader(std::string relative_path_to_json);
    QueryAboutPackage loadPackage(std::string pack_name);
};