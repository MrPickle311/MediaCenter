#pragma once

#include <gmock/gmock.h>

#include"common_utils/MediatorTester.hpp"
#include <Backend.hpp>

struct BackendMediatorsMocks
{
    MediatorMockPtr  data_storage_;
    MediatorMockPtr  environment_;
    MediatorMockPtr  settings_;
    BackendMediatorsMocks():
        data_storage_{new MediatorMock},
        environment_{new MediatorMock},
        settings_{new MediatorMock}
    {}
};

class BackendTEST : public MediatorTester
{
public:
    BackendTEST():
        MediatorTester{"test_data/QueryAboutData.json" , "test_data/RequestActionData.json"}
    {
        BackendBuilder builder;

        builder.addSubsystem("DataStorage" , mocks_.data_storage_)
               .addSubsystem("Environment" , mocks_.environment_)
               .addSubsystem("Settings" , mocks_.settings_);

        builder.addSubsystemBinding("DataStorage" , "Search")
               .addSubsystemBinding("DataStorage" , "Playlist")
               .addSubsystemBinding("Settings"    , "Mediapaths")
               .addSubsystemBinding("Settings"    , "Appdir")
               .setThreadsCount(10);

        tested_mediator_ = builder.build();

        // expect that backend is initilized
        EXPECT_NE(tested_mediator_.get() , nullptr);

        // set dependency
        factories_.setMediator(tested_mediator_);
    }
    virtual ~BackendTEST(){}
protected:
    BackendMediatorsMocks  mocks_;
    //special cases
    void simpleMultimediaEngineTest(std::string call_data ,  std::string precall_data , int calls_count = 1)
    {
        auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , 
                                             factories_.loadQueryPackage(call_data) ,
                                             calls_count)};
        wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , 
                                                   factories_.loadQueryPackage(precall_data) ,
                                                   calls_count));

        appendFunctionWrapperToCallList(std::move(wrapper) , calls_count);

        start();  
    }
};

struct MultimediaEngineMocks
{
    ProxyMockPtr  audio_;
    ProxyMockPtr  video_;
    ProxyMockPtr  images_;
    MediatorMock  backend_;
    MultimediaEngineMocks():
        audio_{new MediatorMock},
        video_{new MediatorMock},
        images_{new MediatorMock}
    {}
};

class MultimediaEngineTests : public MediatorTester
{
public:
    MultimediaEngineTests():
        MediatorTester{"test_data/QueryAboutData.json" , "test_data/RequestActionData.json"}
    {
        MultimediaEngineBuilder builder;

        builder.addSubsystem("Audio" , mocks_.audio_)
               .addSubsystem("Video" , mocks_.video_)
               .addSubsystem("Images" , mocks_.images_)
               .addSubsystem("Backend" , mocks_.backend_);

        tested_mediator_ = builder.build();

        // expect that backend is initilized
        EXPECT_NE(tested_mediator_.get() , nullptr);

        // set dependency
        factories_.setMediator(tested_mediator_);
    }
    virtual ~MultimediaEngineTests() {}
protected:
    MultimediaEngineMocks mocks_;
protected:
    
};