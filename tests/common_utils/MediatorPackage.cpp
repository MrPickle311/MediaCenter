#include "MediatorPackage.hpp"
#include <iostream>
#include <algorithm>

QueryAboutPackage::QueryAboutPackage():
    is_empty_{true}
{
    appendString("command");
    appendStringArray("expectedResult");
    appendStringArray("callArguments");
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
    this->setString("command", cmd);
}

const QString& QueryAboutPackage::getCommand() const 
{
    return this->getString("command"); 
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

void QueryAboutPackage::setCallArguments(const QStringList& args)
{
    this->setPackageNotEmpty();
    this->setStringArray("callArguments" , args);
}

const QStringList& QueryAboutPackage::getCallArguments() const 
{ 
    return this->getStringArray("callArguments");
}

QueryAboutPackageLoader::QueryAboutPackageLoader(std::string relative_path_to_json)
{
    std::filesystem::path project_dir{PROJECT_FOLDER_NAME};
    json_file_.setFileName({ ( project_dir / "data" / relative_path_to_json ).c_str() });

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
    entire_json_object_ = doc_.object();
}

QJsonObject QueryAboutPackageLoader::readPackage(std::string pack_name) const
{
    return entire_json_object_[QString::fromStdString(pack_name)].toObject();
}

QueryAboutPackage QueryAboutPackageLoader::loadPackage(std::string pack_name) 
{
    QJsonObject json_local_obj;
    QueryAboutPackage result;

    if(checker_.jsonObjectContainsPackage(entire_json_object_ , pack_name))
    {
        json_local_obj = readPackage(pack_name);
    }

    if(checker_.isPackageValid( result , json_local_obj  ))
    {
        extractor_.extractPackage(json_local_obj , result );
    }

    return result;
}
