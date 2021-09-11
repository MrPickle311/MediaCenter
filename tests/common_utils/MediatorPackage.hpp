#pragma once

#include <QString>
#include <QStringList>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include <filesystem>
#include <variant>

class IJsonPackage
{
public:
    virtual void appendStringArray(QString name) = 0;
    virtual void appendString(QString name) = 0;

    virtual QStringList getStringArray(QString name) = 0;
    virtual void        setStringArray(QString name , QStringList value) = 0;

    virtual QString     getString(QString name) = 0;
    virtual void        setString(QString name , QString value) = 0;

    virtual QStringList getStringNames() const = 0;
    virtual QStringList getStringArrayNames() const = 0;
};

using IJsonPackagePtr      = std::shared_ptr<IJsonPackage>;
using IJsonPackageConstPtr = std::shared_ptr<const IJsonPackage>;

class JsonPackage : public IJsonPackage
{
    friend class JsonPackageConfigurator;
    using KeyString = QString;
private:
    std::map<KeyString , QString> strings_;
    std::map<KeyString , QStringList> string_arrays_;
private:
    template<typename T>
    QStringList getNames(const T& source) const
    {
        QStringList result;

        for(auto&& name_pair : source)
        {
            result.append(name_pair.first);
        }

        return result;
    }
public:
    void appendStringArray(QString name);
    void appendString(QString name);

    QStringList getStringArray(QString name);
    void        setStringArray(QString name , QStringList value);

    QString     getString(QString name);
    void        setString(QString name , QString value);

    QStringList getStringNames() const;
    QStringList getStringArrayNames() const;
};

class JsonPackageConfigurator
{
private:
    JsonPackage pack_;
public:
    JsonPackageConfigurator& appendStringArray(QString name);
    JsonPackageConfigurator& appendString(QString name);
    JsonPackage buildPack() const;
};

class QueryAboutPackage
{
    friend class QueryAboutPackageFactory;
private:
    bool          is_empty_;
    JsonPackage   data_;
private:
    QueryAboutPackage(){};
private:
    void setPackageNotEmpty();
public:
    QueryAboutPackage(const QueryAboutPackage& other);

    operator bool() const;

    void setCommand(QString cmd);
    const QString& getCommand() const;

    void setExpectedResult(const QStringList& res);
    const QStringList& getExpectedResult() const;

    void setCallArguments(const QStringList& args);
    const QStringList& getCallArguments() const;

    void setAll(const JsonPackage& args);
    const JsonPackage& getAll() const;
};

class QueryAboutPackageFactory
{
private:
    QueryAboutPackage pack_;
public:
    QueryAboutPackageFactory();
    QueryAboutPackage producePack() const;
};


class PackageChecker
{
private:
    bool namesAreCorrect(QStringList names , const QJsonObject& obj) const;
public:
    PackageChecker(IJsonPackageConstPtr pack);
    bool isPackageValid(const IJsonPackage& pack , const QJsonObject& obj) const;
    bool jsonObjectContainsPackage(const QJsonObject& obj , std::string pack_name) const;
};

class PackageExtractor
{
private:
    const QJsonObject& loaded_object_;
private:
    QStringList extractArray(const QJsonObject& obj , QString name) const;
    void extractStringArrays(const QJsonObject& obj , JsonPackage& target) const;
    void extractStrings(const QJsonObject& obj , JsonPackage& target) const;
public:
    PackageExtractor(const QJsonObject& obj);
public:
    void extractPackage(const QJsonObject& obj , JsonPackage& target) const;    
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