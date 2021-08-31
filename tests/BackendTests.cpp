#include "BackendTests.hpp"
#include <QCoreApplication>

//If an operation is performed ,interrupt it and make the following.
//The task in progress  
// State machine ? https://www.boost.org/doc/libs/1_49_0/libs/msm/doc/HTML/examples/SimpleTutorial.cpp

Utils::Utils(std::shared_ptr<Backend> backend):
        ui_mock_(std::move(backend))
{
    QObject::connect(&event_loop_ , &DelayedEventLoop::runned , 
                     &initizal_function_wrapper_ , &QFunctionWrapper::invoke);
    QObject::connect(&ui_mock_ , &UIMock::dataReady , 
                     &event_loop_ , &DelayedEventLoop::killTestEventLoop);
}

UIMock::UIMock(std::shared_ptr<Backend> backend, QObject *parent) :
    QObject(parent),
    backend_(backend)
{
    QObject::connect(this , &UIMock::requestAction , backend_.get() , &Backend::requestAction );
}

QStringList UIMock::queryAbout(QString command,  QStringList args)
{
    emit dataReady();
    return backend_->queryAbout(command , args);
}


BackendTEST::BackendTEST():
    utils_{backend_},
    backend_{nullptr}
{
    BackendBuilder builder;
    
    builder.setDataBackendDependency(mocks_.data_storage_)
           .setEnvironmentDependency(mocks_.environment_)
           .setSettingsDependency(mocks_.settings_)
           .setThreadsCount(5);

    backend_ = builder.build();
}

void BackendTEST::startEventLoop() 
{
    utils_.event_loop_.startTestEventLoop();
}

void BackendTEST::setInitialFunction(std::function<void()> function) 
{
    utils_.initizal_function_wrapper_.setFunction(std::move(function));
}

void BackendTEST::setUIQueryAboutAsInit(QueryAboutPackage call_package) 
{
    std::function<void()> ui_action = [&]()
    {
        //waits until searched QStringList is prepared
        utils_.result = utils_.ui_mock_
                              .queryAbout(call_package.command() ,
                                          call_package.callArguments());

        emit utils_.ui_mock_.dataReady();
    };
 
    setInitialFunction(ui_action);
}

void BackendTEST::expectResultSize(int size) 
{
    EXPECT_EQ(utils_.result.size(),size);
}

void BackendTEST::expectResultElementEqualTo(int idx,QString what) 
{
    EXPECT_STREQ(utils_.result.at(idx).toStdString().c_str(),what.toStdString().c_str());
}

void BackendTEST::invokePrecall(QueryAboutPackage pack)
{
    if(pack)
    {
        auto precall_result = backend_->queryAbout(pack.command() ,
                                                   pack.callArguments());
        
        EXPECT_EQ(precall_result , pack.result());
    }
}

void BackendTEST::expectQueryAboutCall(MediatorMOCK& target ,
                                       QueryAboutPackage call_package,
                                       QueryAboutPackage pre_call_package) 
{
    using ::testing::Eq;

    EXPECT_CALL(target, 
                queryAbout(Eq(call_package.command()),
                           Eq(call_package.callArguments())))
        .WillOnce(
            [&]
            {
                invokePrecall(pre_call_package);
                return call_package.result();
            }
        );
}

// Unit tests

TEST_F(BackendTEST, AudioSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.command()       = "SearchAudio";
    storage_pack.result()        = QStringList{"/home/abc/audio/song3.mp3"};
    storage_pack.callArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "PathsAudio";
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

    storage_pack.command()       = "SearchAudio";
    storage_pack.result()        = QStringList
                                   {
                                       "/home/abc/audio/song1.mp3",
                                       "/home/abc/audio/song2.mp3",
                                       "/home/abc/audio/song3.mp3"
                                   };
    storage_pack.callArguments() = QStringList{"song"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "PathsAudio";
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

    storage_pack.command()       = "SearchAudio";
    storage_pack.result()        = QStringList
                                   {
                                       "/home/abc/audio/vol1/song3.mp3",
                                       "/home/abc/audio/vol2/song3.mp3"
                                   };
    storage_pack.callArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

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

    settings_pack.command() =  "AddDirAudio";
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

    storage_pack.command()       = "SearchVideo";
    storage_pack.result()        = QStringList{"/home/abc/video/video.mp4"};
    storage_pack.callArguments() = QStringList{"video.mp4"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "PathsVideo";
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

    storage_pack.command()  = "PlaylistAudio";
    storage_pack.result()   = QStringList
                              {
                                  "/home/abc/audio/song1.mp3",
                                  "/home/abc/audio/song2.mp3",
                                  "/home/abc/audio/song3.mp3"
                              };

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "PathsAudio";
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

    storage_pack.command()       = "SearchImages";
    storage_pack.result()        = QStringList{"/home/abc/img/image.jpg"};
    storage_pack.callArguments() = QStringList{"image.jpg"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "PathsImages";
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


