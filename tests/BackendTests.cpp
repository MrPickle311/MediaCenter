#include "BackendTests.hpp"
#include <QCoreApplication>

//If an operation is performed ,interrupt it and make the following.
//The task in progress  
// State machine ? https://www.boost.org/doc/libs/1_49_0/libs/msm/doc/HTML/examples/SimpleTutorial.cpp

#include "common_utils/MediatorPackage.hpp"

TEST_F(BackendTEST, AudioSearch)
{
    /*
    QueryAboutPackage storage_pack;

    storage_pack.setCommand()       = "SearchAudio";
    storage_pack.setExpectedResult()        = QStringList{"/home/abc/audio/song3.mp3"};
    storage_pack.setCallArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsAudio";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/audio"};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
    */

    QueryAboutPackageLoader loader{"test_data/BackendTestData.json"};

    QueryAboutPackage pack {loader.loadPackage("AudioSearch1")};

    std::cout << pack.getCallArguments().toStdList().front().toStdString() << std::endl;
    std::cout << pack.getCommand().toStdString() << std::endl;
    std::cout << pack.getExpectedResult().toStdList().front().toStdString() << std::endl;

    pack = loader.loadPackage("MediaPathsAudio1");

    if(pack.getCallArguments().isEmpty())
    {
        std::cout << "is ok \n";
    }

    std::cout << pack.getCommand().toStdString() << std::endl;
    std::cout << pack.getExpectedResult().toStdList().front().toStdString() << std::endl;
}

/*

TEST_F(BackendTEST , AudioNotFullName)
{
    QueryAboutPackage storage_pack;

    storage_pack.setCommand()       = "SearchAudio";
    storage_pack.setExpectedResult()        = QStringList
                                   {
                                       "/home/abc/audio/song1.mp3",
                                       "/home/abc/audio/song2.mp3",
                                       "/home/abc/audio/song3.mp3"
                                   };
    storage_pack.setCallArguments() = QStringList{"song"};

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsAudio";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/audio"};
    
    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}

TEST_F(BackendTEST, AudioMultipleFileName)
{
    QueryAboutPackage storage_pack;

    storage_pack.setCommand()       = "SearchAudio";
    storage_pack.setExpectedResult()        = QStringList
                                   {
                                       "/home/abc/audio/vol1/song3.mp3",
                                       "/home/abc/audio/vol2/song3.mp3"
                                   };
    storage_pack.setCallArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsAudio";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/audio"};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}


TEST_F(BackendTEST, AppendAudioDir)
{
    QString command{"AppdirAudio"};

    appendRequestActionToList(command , mocks_.settings_);

    start();
}

TEST_F(BackendTEST , VideoSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.setCommand()       = "SearchVideo";
    storage_pack.setExpectedResult()        = QStringList{"/home/abc/video/video.mp4"};
    storage_pack.setCallArguments() = QStringList{"video.mp4"};

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsVideo";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/video"};
    
    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}

TEST_F(BackendTEST , AudioPlaylist)
{
    QueryAboutPackage storage_pack;

    storage_pack.setCommand()  = "PlaylistAudio";
    storage_pack.setExpectedResult()   = QStringList
                              {
                                  "/home/abc/audio/song1.mp3",
                                  "/home/abc/audio/song2.mp3",
                                  "/home/abc/audio/song3.mp3"
                              };

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsAudio";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/audio"};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}

TEST_F(BackendTEST , ImageSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.setCommand()       = "SearchImages";
    storage_pack.setExpectedResult()        = QStringList{"/home/abc/img/image.jpg"};
    storage_pack.setCallArguments() = QStringList{"image.jpg"};

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsImages";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/img"};
    
    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}

TEST_F(BackendTEST , UnsupportedCommand)
{
    QueryAboutPackage pack;

    pack.setCommand() = "XYZ";
    pack.setExpectedResult()  = QStringList{"WrongCmd"};

    int expected_calls_count{0};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , pack , expected_calls_count)};

    appendFunctionWrapperToCallList(std::move(wrapper));
    
    start();
}

TEST_F(BackendTEST , MultipleCall)
{
    QueryAboutPackage storage_pack;

    int calls_count{20};

    storage_pack.setCommand()       = "SearchImages";
    storage_pack.setExpectedResult()        = QStringList{"/home/abc/img/image.jpg"};
    storage_pack.setCallArguments() = QStringList{"image.jpg"};

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsImages";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/img"};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack , calls_count)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack , calls_count));

    appendFunctionWrapperToCallList(std::move(wrapper) , calls_count);
    
    start();
}

TEST_F(BackendTEST , MixedCalls)
{
    int calls_count{300};

    QueryAboutPackage storage_pack;

    storage_pack.setCommand()  = "PlaylistAudio";
    storage_pack.setExpectedResult()   = QStringList
                              {
                                  "/home/abc/audio/song1.mp3",
                                  "/home/abc/audio/song2.mp3",
                                  "/home/abc/audio/song3.mp3"
                              };

    QueryAboutPackage settings_pack;

    settings_pack.setCommand() =  "MediapathsAudio";
    settings_pack.setExpectedResult()  =  QStringList{"/home/abc/audio"};

    auto wrapper {createQueryAboutCaller(*mocks_.data_storage_ , storage_pack , calls_count)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack , calls_count));

    appendFunctionWrapperToCallList(std::move(wrapper) , calls_count);

    QString command{"AppdirAudio"};

    appendRequestActionToList(command , mocks_.settings_ , calls_count);

    start();
}

*/

//TEST IDEA MULTIPLE BACKEND CALLS , MIX CALLS

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


