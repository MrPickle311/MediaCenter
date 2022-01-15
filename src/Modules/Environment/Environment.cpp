#include "Environment.hpp"

#include <functional>

IEnvironment::IEnvironment(QObject *parent) : QObject(parent) {}

decltype(auto) Environment::getIconInserter() const {
  using InsertingFunc =
      std::function<void(Environment &, std::string, std::string)>;

  InsertingFunc inserter;

  if (isParentPathSet())
    inserter = &Environment::addRelativeIconPath;
  else
    inserter = &Environment::addIconPath;

  return inserter;
}

void Environment::setParentPath(const std::filesystem::path &parent_path) {
  parent_path_ = parent_path;
}

bool Environment::isParentPathSet() const {
  return parent_path_ != std::filesystem::path{};
}

void Environment::addRelativeIconPath(std::string relative_path,
                                      std::string object_id) {
  frontend_icons_[object_id] =
      parent_path_ / std::filesystem::path{relative_path};
}

void Environment::addNormalIconPath(std::string path, std::string object_id) {
  frontend_icons_[object_id] = path;
}

void Environment::addIconPath(std::string path, std::string object_id) {
  if (isParentPathSet()) {
    addRelativeIconPath(path, object_id);
  } else {
    addNormalIconPath(path, object_id);
  }
}

QString Environment::getIconPath(QString icon_id) {
  return ("qrc:/" + frontend_icons_[icon_id.toStdString()].string()).c_str();
}
