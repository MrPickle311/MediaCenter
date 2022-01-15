#pragma once

#include <QString>
#include <QStringList>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include <map>

class IJsonPackage
{
public:
    virtual void        appendStringArray(QString name) = 0;
    virtual void        appendString(QString name) = 0;

    virtual const QStringList& getStringArray(QString name) const = 0;
    virtual void               setStringArray(QString name , QStringList value) = 0;

    virtual const QString&     getString(QString name) const = 0;
    virtual void               setString(QString name , QString value) = 0;

    virtual QStringList getStringNames() const = 0;
    virtual QStringList getStringArrayNames() const = 0;
};

class JsonPackage : public IJsonPackage
{
    friend class JsonPackageConfigurator;
    using KeyString = QString;
protected:
    std::map<KeyString , QString> strings_;
    std::map<KeyString , QStringList> string_arrays_;
protected:
    void appendStringArray(QString name);
    void appendString(QString name);
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
    virtual const QStringList& getStringArray(QString name) const ;
    void        setStringArray(QString name , QStringList value);

    virtual const QString& getString(QString name) const ;
    void        setString(QString name , QString value);

    QStringList getStringNames() const;
    QStringList getStringArrayNames() const;
};

class IJsonPackageFactory
{
public:
    virtual ~IJsonPackageFactory(){}
    virtual std::shared_ptr<IJsonPackage> produce() const = 0;
};

class JsonPackageConfigurator : IJsonPackageFactory
{
protected:
    std::shared_ptr<JsonPackage> pack_;
public:
    JsonPackageConfigurator& appendStringArray(QString name);
    JsonPackageConfigurator& appendString(QString name);

    virtual std::shared_ptr<IJsonPackage> produce() const;

    std::shared_ptr<JsonPackage> produceConverted() const;
};

class PackageChecker
{
private:
    bool namesAreCorrect(QStringList names , const QJsonObject& obj) const;
public:
    bool isPackageValid(const IJsonPackage& pack , const QJsonObject& obj) const;
    bool jsonObjectContainsPackage(const QJsonObject& obj , std::string pack_name) const;
};

class PackageExtractor
{
private:
    QStringList extractArray(const QJsonObject& obj , QString name) const;
    void extractStringArrays(const QJsonObject& obj , JsonPackage& target) const;
    void extractStrings(const QJsonObject& obj , JsonPackage& target) const;
public:
    void extractPackage(const QJsonObject& obj , JsonPackage& target) const;    
};

