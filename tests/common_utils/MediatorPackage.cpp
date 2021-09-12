#include "MediatorPackage.hpp"
#include <iostream>
#include <algorithm>

MediatorPackage::MediatorPackage():
    is_empty_{true}
{}

void MediatorPackage::setPackageNotEmpty()
{
    this->is_empty_ = false;
}

MediatorPackage::operator bool() const
{
    return !this->is_empty_;
}

CallablePackage::CallablePackage()
{
    appendString("command");
    appendStringArray("callArguments");  
}

void CallablePackage::setCommand(QString cmd)
{
    this->setPackageNotEmpty();
    this->setString("command", cmd);
}

const QString& CallablePackage::getCommand() const 
{
    return this->getString("command"); 
}

void CallablePackage::setCallArguments(const QStringList& args)
{
    this->setPackageNotEmpty();
    this->setStringArray("callArguments" , args);
}

const QStringList& CallablePackage::getCallArguments() const 
{ 
    return this->getStringArray("callArguments");
}

QueryAboutPackage::QueryAboutPackage()
{
    appendString("command");
    appendStringArray("expectedResult");
    appendStringArray("callArguments");
}

void QueryAboutPackage::setExpectedResult(const QStringList& res)
{
    this->setPackageNotEmpty();
    this->setStringArray("expectedResult" , res);
}

const QStringList& QueryAboutPackage::getExpectedResult() const 
{ 
    return this->getStringArray("expectedResult");
}

JsonFile::JsonFile(QString path):
    json_file_{path}
{
    if(!path.isEmpty())
    {
        this->tryOpenJsonFile();
    } 
}

void JsonFile::printError(std::string msg) const
{
    std::cout << "\n " << msg << " \n";
}

void JsonFile::checkIfFileExists() const
{
    if(!json_file_.exists())
    {
        printError("The JSON file does not exist!");
    }
}

void JsonFile::tryOpenJsonFile() 
{
    this->checkIfFileExists();

    if(!json_file_.open(QIODevice::ReadOnly))
    {
        printError("Couldn't open the JSON file!");
    }
}

QJsonObject JsonFile::tryLoadJsonObject() 
{
    return QJsonDocument::fromJson(json_file_.readAll()).object();
}

void JsonFile::setPath(QString path) 
{
    this->json_file_.setFileName(path);
    tryOpenJsonFile();
}

JsonPackageLoader::JsonPackageLoader(std::string relative_path_to_json)
{
    std::filesystem::path project_dir{PROJECT_FOLDER_NAME};

    this->file_.setPath({ ( project_dir / "data" / relative_path_to_json ).c_str() });

    entire_json_object_ = file_.tryLoadJsonObject();
}

QJsonObject JsonPackageLoader::readPackage(std::string pack_name) const
{
    return entire_json_object_[QString::fromStdString(pack_name)].toObject();
}

QJsonObject JsonPackageLoader::tryLoadLocalPackage(std::string pack_name) const
{
    if(checker_.jsonObjectContainsPackage(entire_json_object_ , pack_name))
    {
        return readPackage(pack_name);
    }

    return {};
}

void JsonPackageLoader::tryExtractResult(const QJsonObject& obj , JsonPackage& pack) 
{
    if(checker_.isPackageValid( pack , obj  ))
    {
        extractor_.extractPackage(obj , pack );
    }
}

void JsonPackageLoader::tryExtract(JsonPackage& result , std::string pack_name) 
{
    auto json_local_obj{tryLoadLocalPackage(pack_name)};
    tryExtractResult(json_local_obj , result);
}

QueryAboutPackage QueryAboutPackageLoader::loadPackage(std::string pack_name) 
{
    QueryAboutPackage result;

    tryExtract(result , pack_name);
    
    return result;
}

RequestActionPackage RequestActionPackageLoader::loadPackage(std::string pack_name) 
{
    RequestActionPackage result;

    tryExtract(result , pack_name);

    return result;
}
