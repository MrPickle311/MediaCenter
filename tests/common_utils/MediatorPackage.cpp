#include "MediatorPackage.hpp"
#include <iostream>

QueryAboutPackage::QueryAboutPackage():
    is_empty_{true},
    call_args_{}
{}

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

QueryAboutPackageLoader::QueryAboutPackageLoader(std::string relative_path_to_json):
    json_file_{QString::fromStdString(std::filesystem::current_path().string() + relative_path_to_json)}
{
    this->checkIfFileExists();
    this->tryOpenJsonFile();
    this->tryLoadJsonFile();
}

void QueryAboutPackageLoader::printError(std::string msg) 
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

bool QueryAboutPackageLoader::isJsonPackValid(const QJsonObject& obj) const
{
    return obj.contains("command") && 
           obj.contains("expectedResult") && 
           obj.contains("callArguments");
}

QueryAboutPackage QueryAboutPackageLoader::extractQueryPackage(const QJsonObject& obj) const
{
    QueryAboutPackage pack;

    pack.setCommand(obj["command"]);
    pack.setExpectedResult(obj["expectedResult"]);
    pack.setCallArguments(obj["callArguments"]);

    return pack;
}

bool QueryAboutPackageLoader::containsPackage(std::string pack_name) const
{
    QString temp_str{QString::fromStdString(pack_name)};

    return loaded_object_.contains(temp_str) &&
           loaded_object_[temp_str].isObject();
}

QJsonObject QueryAboutPackageLoader::readPackage(std::string pack_name) const
{
    return loaded_object_[QString::fromStdString(pack_name)].toObject();
}

QueryAboutPackage QueryAboutPackageLoader::loadPackage(std::string pack_name) 
{
    QJsonObject json_pack;

    if(this->containsPackage(pack_name))
    {
        json_pack = readPackage(pack_name);
    }

    if(this->isJsonPackValid(json_pack))
    {
        return extractQueryPackage(json_pack);
    }

    return {};
}