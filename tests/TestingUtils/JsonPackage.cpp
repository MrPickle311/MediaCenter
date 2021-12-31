#include "JsonPackage.hpp"


void JsonPackage::appendStringArray(QString name) 
{   
    string_arrays_[name] = QStringList{};
}

void JsonPackage::appendString(QString name) 
{
    strings_[name] = QString{};
}

const QStringList& JsonPackage::getStringArray(QString name) const 
{
    return string_arrays_.at(name);
}

void JsonPackage::setStringArray(QString name , QStringList value) 
{
    string_arrays_.at(name) = value;
}

const QString& JsonPackage::getString(QString name) const 
{
    return strings_.at(name);
}

void JsonPackage::setString(QString name , QString value) 
{
    strings_.at(name) = value;
}

QStringList JsonPackage::getStringNames() const
{
   return getNames(strings_);
}

QStringList JsonPackage::getStringArrayNames() const
{
    return getNames(string_arrays_);
}

JsonPackageConfigurator& JsonPackageConfigurator::appendStringArray(QString name) 
{
    pack_->appendStringArray(name);
    return *this;
}

JsonPackageConfigurator& JsonPackageConfigurator::appendString(QString name) 
{
    pack_->appendString(name);
    return *this;
}

std::shared_ptr<IJsonPackage> JsonPackageConfigurator::produce() const
{
    return std::make_shared<JsonPackage>(*pack_);//copy
}

std::shared_ptr<JsonPackage> JsonPackageConfigurator::produceConverted() const
{
    return std::dynamic_pointer_cast<JsonPackage>(produce());//cast down to JsonPackage
}

bool PackageChecker::namesAreCorrect(QStringList names , const QJsonObject& obj) const
{
    return std::all_of(names.begin(), names.end() ,
            [&obj](const QString& el)
            {
                return obj.contains(el);
            });
}

bool PackageChecker::isPackageValid(const IJsonPackage& pack , const QJsonObject& obj) const
{
    return namesAreCorrect(pack.getStringNames() , obj) &&
           namesAreCorrect(pack.getStringArrayNames() , obj);
}

bool PackageChecker::jsonObjectContainsPackage(const QJsonObject& obj , std::string pack_name) const
{
    QString pkg_name{QString::fromStdString(pack_name)};

    return obj.contains(pkg_name) && obj[pkg_name].isObject();
}

QStringList PackageExtractor::extractArray(const QJsonObject& obj , QString name) const
{
    QStringList result;

    for(auto&& el : obj[name].toArray())
    {
        result.append(el.toString());
    }

    return result;
}

void PackageExtractor::extractStringArrays(const QJsonObject& obj , JsonPackage& target) const
{
    for(auto&& name : target.getStringArrayNames())
    {
        target.setStringArray(name , extractArray(obj , name));
    }
}

void PackageExtractor::extractStrings(const QJsonObject& obj , JsonPackage& target) const
{
    for(auto&& name : target.getStringNames())
    {
        target.setString(name , obj[name].toString());
    }
}

void PackageExtractor::extractPackage(const QJsonObject& obj , JsonPackage& target) const
{
    extractStrings(obj , target);
    extractStringArrays(obj , target);
}