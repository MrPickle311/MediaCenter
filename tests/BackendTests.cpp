#include "BackendTests.hpp"
#include <QCoreApplication>

//Jeśli operacja szukania jest wykonywana ,to przerwij ją i rób tę ,co nadesłałem
//Co jakiś czas niech sprawdza flagę , czy nie należy przerwać zadania.
// State machine ? https://www.boost.org/doc/libs/1_49_0/libs/msm/doc/HTML/examples/SimpleTutorial.cpp

TEST_F(BackendTEST, Stability)
{
    expectQueryAboutCall(mocks_.settings_,"DataStorage","Paths",{"/home/abc/audio"});
    expectQueryAboutCall(mocks_.data_storage_,"Audio","Search",
                         {"/home/abc/audio/song3.mp3"},{"song3.mp3"});
    
    setUIQueryAboutAsInit("Audio","Search",{QString{"song3.mp3"}});
    
    startEventLoop();

    expectResultSize(1);
    expectResultElementEqualTo(0,"/home/abc/audio/song3.mp3");
}

TEST_F(BackendTEST , AudioNotFullName)
{
    expectQueryAboutCall(mocks_.settings_,"DataStorage","Paths",{"/home/abc/audio"});
                        
    expectQueryAboutCall(mocks_.data_storage_,"Audio","Search",
                        {"/home/abc/audio/song1.mp3",
                         "/home/abc/audio/song2.mp3",
                         "/home/abc/audio/song3.mp3"}
                        ,{"song"});

    setUIQueryAboutAsInit("Audio","Search",{QString{"song"}});

    startEventLoop();

    expectResultSize(3);
    expectResultElementEqualTo(0,"/home/abc/audio/song1.mp3");
    expectResultElementEqualTo(1,"/home/abc/audio/song2.mp3");
    expectResultElementEqualTo(2,"/home/abc/audio/song3.mp3");
}

TEST_F(BackendTEST, AudioMultipleFileName)
{
    expectQueryAboutCall(mocks_.settings_,"DataStorage","Paths",{"/home/abc/audio"});

    expectQueryAboutCall(mocks_.data_storage_,"Audio","Search",
                        {"/home/abc/audio/vol1/song3.mp3",
                         "/home/abc/audio/vol2/song3.mp3"}
                        ,{"song3.mp3"});

    setUIQueryAboutAsInit("Audio","Search",{QString{"song3.mp3"}});

    startEventLoop();

    expectResultSize(2);
    expectResultElementEqualTo(0,"/home/abc/audio/vol1/song3.mp3");
    expectResultElementEqualTo(1,"/home/abc/audio/vol2/song3.mp3");
}

TEST_F(BackendTEST, AddAudioDir)
{
    expectQueryAboutCall(mocks_.settings_,"DataStorage","Paths",{"/home/abc/audio"});

    setUIQueryAboutAsInit("Audio","AddDir",{{"/home/abc/audio"}});

    QObject::connect(&mocks_.settings_,&MediatorMOCK::requestAction ,
            [this](QString sender,QString requestedAction,QVariantList args = {})
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
    expectQueryAboutCall(mocks_.settings_,"Video","Paths",{"/home/abc/video"});

    expectQueryAboutCall(mocks_.data_storage_,"Video","Search",
                        {"/home/abc/video/video.mp4"},
                        {"video.mp4"});

    setUIQueryAboutAsInit("Video","Search",{{"video.mp4"}});

    startEventLoop();

    expectResultSize(1);
    expectResultElementEqualTo(0,"/home/abc/video/video.mp4");
}

TEST_F(BackendTEST , AudioPlaylist)
{
    expectQueryAboutCall(mocks_.settings_,"Audio","Path",{"/home/abc/audio"});

    expectQueryAboutCall(mocks_.data_storage_,"Audio","Playlist",
                        {"/home/abc/audio/song1.mp3",
                         "/home/abc/audio/song2.mp3",
                         "/home/abc/audio/song3.mp3"});

    setUIQueryAboutAsInit("Audio","Playlist");

    startEventLoop();

    expectResultSize(3);
    expectResultElementEqualTo(0,"/home/abc/audio/song1.mp3");
    expectResultElementEqualTo(1,"/home/abc/audio/song2.mp3");
    expectResultElementEqualTo(2,"/home/abc/audio/song3.mp3");
}

TEST_F(BackendTEST , ImageSearch)
{
    expectQueryAboutCall(mocks_.settings_,"Image","Paths",{"/home/abc/img"});

    expectQueryAboutCall(mocks_.data_storage_,"Image","Search",
                        {"/home/abc/img/image.jpg"},
                        {"image.jpg"});

    setUIQueryAboutAsInit("Image","Search",{{"image.jpg"}});

    startEventLoop();

    expectResultSize(1);
    expectResultElementEqualTo(0,"/home/abc/img/image.jpg");
}

TEST_F(BackendTEST , UnsupportedCommand)
{
    setUIQueryAboutAsInit("Audio","XYZ");

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


