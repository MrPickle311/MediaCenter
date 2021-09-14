#pragma once

#include <Mediator.hpp>

class MultimediaEngine : public IMediator
{
public:
    virtual ~MultimediaEngine(){}
public:
    virtual QStringList queryAbout(const QString& command, QStringList args);
};