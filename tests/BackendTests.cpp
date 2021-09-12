#include "BackendTests.hpp"
#include <QCoreApplication>

#include "common_utils/MediatorPackage.hpp"


TEST_F(BackendTEST, AudioSearch)
{
    simpleTest("AudioSearch1" , "MediaPathsAudio1");
}

TEST_F(BackendTEST , AudioNotFullName)
{
    simpleTest("AudioSearch2" , "MediaPathsAudio1");
}

TEST_F(BackendTEST, AudioMultipleFileName)
{
    simpleTest("AudioSearch3" , "MediaPathsAudio1");
}

TEST_F(BackendTEST, AppendAudioDir)
{
    appendRequestActionToList("AppdirAudio" , mocks_.settings_);
    start();
}

TEST_F(BackendTEST , VideoSearch)
{
    simpleTest("VideoSearch1" , "MediaPathsVideo1");
}


TEST_F(BackendTEST , AudioPlaylist)
{
    simpleTest("AudioPlaylist1" , "MediaPathsAudio1");
}



TEST_F(BackendTEST , ImageSearch)
{
    simpleTest("ImagesSearch1" , "MediaPathsImages1");
}


TEST_F(BackendTEST , UnsupportedCommand)
{
    QueryAboutPackage pack{query_loader_.loadPackage("UnsupportedCommand1")};

    int expected_calls_count{0};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , pack , expected_calls_count)};

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}


TEST_F(BackendTEST , MultipleCall)
{
    int calls_count{20};

    simpleTest("ImagesSearch1" , "MediaPathsImages1" , calls_count);
}

TEST_F(BackendTEST , MixedCalls)
{
    int calls_count{300};

    QueryAboutPackage storage_pack{query_loader_.loadPackage("AudioPlaylist1")};

    QueryAboutPackage settings_pack{query_loader_.loadPackage("MediaPathsAudio1")};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack , calls_count)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack , calls_count));

    appendFunctionWrapperToCallList(std::move(wrapper) , calls_count);

    appendRequestActionToList("AppdirAudio" , mocks_.settings_ , calls_count);

    start();
}



//TEST IDEA MULTIPLE BACKEND CALLS , MIX CALLS

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


