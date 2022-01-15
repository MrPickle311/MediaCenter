#pragma once

#include <Mediator.hpp>


class MultimediaEngine : public Mediator
{
    friend class MultimediaEngineBuilder;
};

using MultimediaEnginePtr = std::shared_ptr<MultimediaEngine>;

class MultimediaEngineBuilder : public MediatorBuilder
{
public:
    MultimediaEnginePtr buildMultimediaEngine();
};