#include "DataBackendTests.hpp"
#include <QCoreApplication>

TEST_F(DataBackendMediatorTests , LogicTests)
{
    QueryAboutPackage audio_pack;

    audio_pack.command()       = "SearchAudio";
    audio_pack.expectedResult()        = QStringList{
                                                        "/home/abc/audio1/song3.mp3" ,
                                                        "/home/abc/audio2/song3.mp3"
                                                    };
    audio_pack.callArguments() = QStringList{"song3.mp3"};


    QueryAboutPackage video_pack;

    video_pack.command()       = "SearchVideo";
    video_pack.expectedResult()        = QStringList{
                                                        "/home/abc/video1/video.mp4" ,
                                                        "/home/abc/video2/video.mp4"
                                                    };
    video_pack.callArguments() = QStringList{"video.mp4"};


    QueryAboutPackage image_pack;

    video_pack.command()       = "SearchImages";
    video_pack.expectedResult()        = QStringList{
                                                        "/home/abc/imeages1/img.jpeg" ,
                                                        "/home/abc/imeages2/img.jpeg"
                                                    };
    video_pack.callArguments() = QStringList{"img.jpeg"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "MediapathsAudio";
    settings_pack.expectedResult()  =  QStringList{
                                                    "/home/abc/audio1" ,
                                                    "/home/abc/audio2"
                                                  };

    auto wrapper {createQueryAboutCaller(*mocks_.audio , audio_pack)};
    wrapper->setPrecall(createQueryAboutCaller(*mocks_.settings_  , settings_pack));
    appendFunctionWrapperToCallList(wrapper);





    
    start();
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}