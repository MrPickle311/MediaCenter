#include "MediatorPackage.hpp"
#include <iostream>
#include <algorithm>

QueryAboutPackage::QueryAboutPackage():
    is_empty_{true}
{

}

void QueryAboutPackage::setPackageNotEmpty()
{
    this->is_empty_ = false;
}

QueryAboutPackage::operator bool() const
{
    return !this->is_empty_;
}

void QueryAboutPackage::setCommand(QString cmd)
{
    this->setPackageNotEmpty();
    this->command_ = cmd; 
}

const QString& QueryAboutPackage::getCommand() const 
{ 
    return this->command_; 
}

void QueryAboutPackage::setExpectedResult(const QStringList& res)
{
    this->setPackageNotEmpty();
    this->expected_result_ = res; 
}

const QStringList& QueryAboutPackage::getExpectedResult() const 
{ 
    return this->expected_result_; 
}

void QueryAboutPackage::setCallArguments(const QStringList& args)
{
    this->setPackageNotEmpty();
    this->call_args_ = args; 
}

const QStringList& QueryAboutPackage::getCallArguments() const 
{ 
    return this->call_args_; 
}

QueryAboutPackageFactory::QueryAboutPackageFactory() 
{
    pack_.data_.appendString("command");
    pack_.data_.appendStringArray("expectedResult");
    pack_.data_.appendStringArray("callArguments");
}

QueryAboutPackage QueryAboutPackageFactory::producePack() const
{
    return pack_;
}

QueryAboutPackageLoader::QueryAboutPackageLoader(std::string relative_path_to_json):
    extractor_{loaded_object_}
{
    std::filesystem::path curr_path{PROJECT_FOLDER_NAME};
    curr_path /= "data";
    curr_path /= relative_path_to_json;
    json_file_.setFileName(QString::fromStdString(curr_path.string()));

    this->checkIfFileExists();
    this->tryOpenJsonFile();
    this->tryLoadJsonFile();
}

void QueryAboutPackageLoader::printError(std::string msg) const
{
    std::cout << "\n " << msg << " \n";
}

void QueryAboutPackageLoader::checkIfFileExists() const
{
    if(!json_file_.exists())
    {
        printError("The JSON file does not exist!");
    }
}

void QueryAboutPackageLoader::tryOpenJsonFile() 
{
    if(!json_file_.open(QIODevice::ReadOnly))
    {
        printError("Couldn't open the JSON file!");
    }
}

void QueryAboutPackageLoader::tryLoadJsonFile() 
{
    doc_ = QJsonDocument::fromJson(json_file_.readAll());
    loaded_object_ = doc_.object();
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

QJsonObject QueryAboutPackageLoader::readPackage(std::string pack_name) const
{
    return entire_json_object_[QString::fromStdString(pack_name)].toObject();
}

QueryAboutPackage QueryAboutPackageLoader::loadPackage(std::string pack_name) 
{
    QJsonObject json_local_obj;

    if(checker_.jsonObjectContainsPackage(entire_json_object_ , pack_name))
    {
        json_local_obj = readPackage(pack_name);
    }

    if(checker_.isPackageValid(  , json_local_obj ))
    {
        return extractor_.extractPackage(json_local_obj);
    }

    return {};
}
void JsonPackage::appendStringArray(QString name) 
{
    string_arrays_.insert(name , {});
}

void JsonPackage::appendString(QString name) 
{
    strings_.insert(name , {});
}

QStringList JsonPackage::getStringArray(QString name) 
{
    return string_arrays_.at(name);
}

void JsonPackage::setStringArray(QString name , QStringList value) 
{
    string_arrays_.at(name) = value;
}

QString JsonPackage::getString(QString name) 
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
    pack_.appendStringArray(name);
    return *this;
}

JsonPackageConfigurator& JsonPackageConfigurator::appendString(QString name) 
{
    pack_.appendString(name);
    return *this;
}

JsonPackage JsonPackageConfigurator::buildPack() const
{
    return pack_;
}
