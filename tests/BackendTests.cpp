#include "BackendTests.hpp"
#include <QCoreApplication>

#include "common_utils/MediatorPackage.hpp"


TEST_F(BackendTEST, AudioSearch)
{
    simpleMultimediaEngineTest("AudioSearch1" , "MediaPathsAudio1");
}

TEST_F(BackendTEST , AudioNotFullName)
{
    simpleMultimediaEngineTest("AudioSearch2" , "MediaPathsAudio1");
}

TEST_F(BackendTEST, AudioMultipleFileName)
{
    simpleMultimediaEngineTest("AudioSearch3" , "MediaPathsAudio1");
}

TEST_F(BackendTEST, AppendAudioDir)
{
    appendRequestActionToList( factories_.loadRequestPackage("AppendAudioDir") , 
                               mocks_.settings_);
    start();
}

TEST_F(BackendTEST , VideoSearch)
{
    simpleMultimediaEngineTest("VideoSearch1" , "MediaPathsVideo1");
}


TEST_F(BackendTEST , AudioPlaylist)
{
    simpleMultimediaEngineTest("AudioPlaylist1" , "MediaPathsAudio1");
}



TEST_F(BackendTEST , ImageSearch)
{
    simpleMultimediaEngineTest("ImagesSearch1" , "MediaPathsImages1");
}


TEST_F(BackendTEST , UnsupportedCommand)
{
    QueryAboutPackage pack{factories_.loadQueryPackage("UnsupportedCommand1")};

    int expected_calls_count{0};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , pack , expected_calls_count)};

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}


TEST_F(BackendTEST , MultipleCall)
{
    int calls_count{20};

    simpleMultimediaEngineTest("ImagesSearch1" , "MediaPathsImages1" , calls_count);
}

TEST_F(BackendTEST , MixedCalls)
{
    int calls_count{300};

    QueryAboutPackage storage_pack{factories_.loadQueryPackage("AudioPlaylist1")};

    QueryAboutPackage settings_pack{factories_.loadQueryPackage("MediaPathsAudio1")};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack , calls_count)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack , calls_count));

    appendFunctionWrapperToCallList(std::move(wrapper) , calls_count);

    appendRequestActionToList(factories_.loadRequestPackage("AppendAudioDir") , mocks_.settings_ , calls_count);

    start();
}



//TEST IDEA MULTIPLE BACKEND CALLS , MIX CALLS

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


