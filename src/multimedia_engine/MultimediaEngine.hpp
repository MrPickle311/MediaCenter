#pragma once

#include <Mediator.hpp>


class MultimediaEngine : public Mediator
{
    friend class MultimediaEngineBuilder;
private:
    MultimediaEngine(){}
public:
    virtual ~MultimediaEngine(){}
};

using MultimediaEnginePtr = std::shared_ptr<MultimediaEngine>;

class MultimediaEngineBuilder : public MediatorBuilder
{
public:
    virtual ~MultimediaEngineBuilder(){}
    MultimediaEnginePtr buildMultimediaEngine();
};