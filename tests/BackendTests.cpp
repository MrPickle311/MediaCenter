#include "BackendTests.hpp"
#include <QCoreApplication>

//If an operation is performed ,interrupt it and make the following.
//The task in progress  
// State machine ? https://www.boost.org/doc/libs/1_49_0/libs/msm/doc/HTML/examples/SimpleTutorial.cpp

TEST_F(BackendTEST, AudioSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.sender()        = "Audio";
    storage_pack.command()       = "Search";
    storage_pack.result()        = QStringList{"/home/abc/audio/song3.mp3"};
    storage_pack.callArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "DataStorage";
    settings_pack.command() =  "AudioPaths";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};
    
    expectQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);
    expectQueryAboutCall(*mocks_.settings_ , settings_pack);

    setUIQueryAboutAsInit(storage_pack);
    
    startEventLoop();

    expectResultSize(1);
    expectResultElementEqualTo(0,"/home/abc/audio/song3.mp3");
}

TEST_F(BackendTEST , AudioNotFullName)
{
    QueryAboutPackage storage_pack;

    storage_pack.sender()        = "Audio";
    storage_pack.command()       = "Search";
    storage_pack.result()        = QStringList
                                   {
                                       "/home/abc/audio/song1.mp3",
                                       "/home/abc/audio/song2.mp3",
                                       "/home/abc/audio/song3.mp3"
                                   };
    storage_pack.callArguments() = QStringList{"song"};

    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "DataStorage";
    settings_pack.command() =  "AudioPaths";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};
    
    expectQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);
    expectQueryAboutCall(*mocks_.settings_ , settings_pack);

    setUIQueryAboutAsInit(storage_pack);

    startEventLoop();

    expectResultSize(3);
    expectResultElementEqualTo(0,"/home/abc/audio/song1.mp3");
    expectResultElementEqualTo(1,"/home/abc/audio/song2.mp3");
    expectResultElementEqualTo(2,"/home/abc/audio/song3.mp3");
}

TEST_F(BackendTEST, AudioMultipleFileName)
{
    QueryAboutPackage storage_pack;

    storage_pack.sender()        = "Audio";
    storage_pack.command()       = "Search";
    storage_pack.result()        = QStringList
                                   {
                                       "/home/abc/audio/vol1/song3.mp3",
                                       "/home/abc/audio/vol2/song3.mp3"
                                   };
    storage_pack.callArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "DataStorage";
    settings_pack.command() =  "AudioPaths";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    expectQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);
    expectQueryAboutCall(*mocks_.settings_ , settings_pack);

    setUIQueryAboutAsInit(storage_pack);

    startEventLoop();

    expectResultSize(2);
    expectResultElementEqualTo(0,"/home/abc/audio/vol1/song3.mp3");
    expectResultElementEqualTo(1,"/home/abc/audio/vol2/song3.mp3");
}

TEST_F(BackendTEST, AddAudioDir)
{
    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "Audio";
    settings_pack.command() =  "AddDir";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    setUIQueryAboutAsInit(settings_pack);

    //this lambda checks the requestAction() signal emission
    //place it in separate object called wrapper
    QObject::connect(mocks_.settings_.get() , &MediatorMOCK::requestAction ,
            [this](QString sender,QString requestedAction,QStringList args = {})
            {
                utils_.result.append(sender);
                utils_.result.append(requestedAction);
                emit utils_.event_loop_.killTestEventLoop();
            });
    
    startEventLoop();

    expectResultElementEqualTo(0,"Audio");
    expectResultElementEqualTo(1,"AddDir");
}

TEST_F(BackendTEST , VideoSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.sender()        = "Video";
    storage_pack.command()       = "Search";
    storage_pack.result()        = QStringList{"/home/abc/video/video.mp4"};
    storage_pack.callArguments() = QStringList{"video.mp4"};

    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "DataStorage";
    settings_pack.command() =  "VideoPaths";
    settings_pack.result()  =  QStringList{"/home/abc/video"};
    
    expectQueryAboutCall(*mocks_.settings_ , settings_pack);
    expectQueryAboutCall(*mocks_.data_storage_ , storage_pack);

    setUIQueryAboutAsInit(settings_pack);

    startEventLoop();

    expectResultSize(1);
    expectResultElementEqualTo(0,"/home/abc/video/video.mp4");
}

TEST_F(BackendTEST , AudioPlaylist)
{
    QueryAboutPackage storage_pack;

    storage_pack.sender()   = "Audio";
    storage_pack.command()  = "Playlist";
    storage_pack.result()   = QStringList
                              {
                                  "/home/abc/audio/song1.mp3",
                                  "/home/abc/audio/song2.mp3",
                                  "/home/abc/audio/song3.mp3"
                              };

    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "DataStorage";
    settings_pack.command() =  "AudioPaths";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    expectQueryAboutCall(*mocks_.settings_ , settings_pack);

    expectQueryAboutCall(*mocks_.data_storage_ , storage_pack);

    setUIQueryAboutAsInit(storage_pack);

    startEventLoop();

    expectResultSize(3);
    expectResultElementEqualTo(0,"/home/abc/audio/song1.mp3");
    expectResultElementEqualTo(1,"/home/abc/audio/song2.mp3");
    expectResultElementEqualTo(2,"/home/abc/audio/song3.mp3");
}

TEST_F(BackendTEST , ImageSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.sender()        = "Image";
    storage_pack.command()       = "Search";
    storage_pack.result()        = QStringList{"/home/abc/img/image.jpg"};
    storage_pack.callArguments() = QStringList{"image.jpg"};

    QueryAboutPackage settings_pack;

    settings_pack.sender()  =  "DataStorage";
    settings_pack.command() =  "AudioPaths";
    settings_pack.result()  =  QStringList{"/home/abc/img"};
    
    expectQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);
    expectQueryAboutCall(*mocks_.settings_ , settings_pack);

    setUIQueryAboutAsInit(storage_pack);

    startEventLoop();

    expectResultSize(1);
    expectResultElementEqualTo(0 , storage_pack.result()[0]);
}

TEST_F(BackendTEST , UnsupportedCommand)
{
    QueryAboutPackage pack;

    pack.sender()        = "Audio";
    pack.command()       = "XYZ";

    setUIQueryAboutAsInit(pack);

    startEventLoop();

    expectResultSize(0);
    expectResultElementEqualTo(0,"WrongCmd");
}

//TEST IDEA 

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


