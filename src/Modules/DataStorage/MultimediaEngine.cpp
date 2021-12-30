#include "MultimediaEngine.hpp"

#include <iostream>
MultimediaEnginePtr MultimediaEngineBuilder::buildMultimediaEngine() 
{
    MultimediaEnginePtr multimedia_engine {new MultimediaEngine};

    this->subsystems_->setDesiredParserPos(1);
    multimedia_engine->subsystems_ = std::move(this->subsystems_);
    this->resetSubsystems();

    return multimedia_engine;
}