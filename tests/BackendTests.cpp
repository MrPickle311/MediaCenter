#include "BackendTests.hpp"
#include <QCoreApplication>

//If an operation is performed ,interrupt it and make the following.
//The task in progress  
// State machine ? https://www.boost.org/doc/libs/1_49_0/libs/msm/doc/HTML/examples/SimpleTutorial.cpp

UIMock::UIMock(QObject *parent) :
    QObject(parent),
    backend_(nullptr)
{}

QStringList UIMock::queryAbout(QString command,  QStringList args)
{
    return backend_->queryAbout(command , args);
}

void UIMock::setBackend(std::shared_ptr<Backend> backend) 
{
    this->backend_ = backend;
    QObject::connect(this , &UIMock::requestAction , backend_.get() , &Backend::requestAction );
}

Utils::Utils(std::shared_ptr<Backend> backend)
{
    QObject::connect(&event_loop_ , &DelayedEventLoop::runned , 
                     &wrappers_ , &WrappersList::callAll);
    QObject::connect(&wrappers_ , &WrappersList::finished , 
                     &event_loop_ , &DelayedEventLoop::killTestEventLoop);
}

void Utils::setBackend(std::shared_ptr<Backend> backend)
{
    ui_mock_.setBackend(backend);
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

    EXPECT_NE(backend_.get() , nullptr);

    utils_.setBackend(backend_);
}

void BackendTEST::startEventLoop() 
{
    utils_.event_loop_.startTestEventLoop();
}

void BackendTEST::appendInitialFunction(std::function<void()> function, int count ) 
{
    for(int i{0}; i < count; ++i)
    {
        utils_.wrappers_.append(function);
    }
}

void BackendTEST::appendUIQueryAboutOnStart(QueryAboutPackage call_package , int count ) 
{
    std::function<void()> ui_action = [call_package, this]
    {
        //waits until searched QStringList is prepared
        auto result = utils_.ui_mock_
                              .queryAbout(call_package.command() ,
                                          call_package.callArguments());
        
        checkResult(result , call_package);
    };

    appendInitialFunction(ui_action , count);
}

void BackendTEST::invokePrecall(QueryAboutPackage pack)
{
    if(pack)
    {
        auto precall_result = backend_->queryAbout(pack.command() ,
                                                   pack.callArguments());
        
        checkResult(precall_result , pack);
    }
}

void BackendTEST::expectQueryAboutCall(MediatorMOCK& target ,
                                       int times , 
                                       QueryAboutPackage call_package,
                                       QueryAboutPackage pre_call_package) 
{
    using ::testing::Eq;

    EXPECT_CALL(target, 
                queryAbout(Eq(call_package.command()),
                           Eq(call_package.callArguments())))
        .Times(times)
        .WillRepeatedly(
            [call_package , pre_call_package , this]
            {
                invokePrecall(pre_call_package);
                return call_package.result();
            }
        );
}

void BackendTEST::expectSingleQueryAboutCall(MediatorMOCK& target ,
                                    QueryAboutPackage call_package ,
                                    QueryAboutPackage pre_call_package) 
{
    expectQueryAboutCall(target , 1 , std::move(call_package) , std::move(pre_call_package));
}

void BackendTEST::checkResult(const QStringList& result , const QueryAboutPackage& pattern) 
{
    checker_.pattern() = pattern.result();
    checker_.checkResult(result);
}

void BackendTEST::testQueryAboutCall(QueryAboutPackage call_pack , int count) 
{
    appendUIQueryAboutOnStart(call_pack , count);
    startEventLoop();
}

// Unit tests

TEST_F(BackendTEST, AudioSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.command()       = "SearchAudio";
    storage_pack.result()        = QStringList{"/home/abc/audio/song3.mp3"};
    storage_pack.callArguments() = QStringList{"song3.mp3"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "MediapathsAudio";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    expectSingleQueryAboutCall(*mocks_.data_storage_ ,storage_pack , settings_pack);
    expectSingleQueryAboutCall(*mocks_.settings_ ,  settings_pack);
    
    testQueryAboutCall(storage_pack);
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

    settings_pack.command() =  "MediapathsAudio";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};
    
    expectSingleQueryAboutCall(*mocks_.data_storage_  , storage_pack , settings_pack);
    expectSingleQueryAboutCall(*mocks_.settings_  , settings_pack);

    testQueryAboutCall(storage_pack);
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

    settings_pack.command() =  "MediapathsAudio";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    expectSingleQueryAboutCall(*mocks_.data_storage_  , storage_pack , settings_pack);
    expectSingleQueryAboutCall(*mocks_.settings_ ,  settings_pack);

    testQueryAboutCall(storage_pack);
}

TEST_F(BackendTEST, DISABLED_AppendAudioDir)
{
    QueryAboutPackage settings_pack;

    settings_pack.command() =  "AppdirAudio";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    appendUIQueryAboutOnStart(settings_pack);

    QString result;

    //this lambda checks the requestAction() signal emission
    //place it in separate object called wrapper
    QObject::connect(mocks_.settings_.get() , &MediatorMOCK::requestAction ,
            [this, &result](QString requestedAction,QStringList args = {})
            {
                result = requestedAction;
                emit utils_.event_loop_.killTestEventLoop();
            });
    
    startEventLoop();

    EXPECT_STREQ(result.toStdString().c_str() , "AppdirAudio");
}

TEST_F(BackendTEST , VideoSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.command()       = "SearchVideo";
    storage_pack.result()        = QStringList{"/home/abc/video/video.mp4"};
    storage_pack.callArguments() = QStringList{"video.mp4"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "MediapathsVideo";
    settings_pack.result()  =  QStringList{"/home/abc/video"};
    
    expectSingleQueryAboutCall(*mocks_.settings_ , settings_pack);
    expectSingleQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);

    testQueryAboutCall(storage_pack);
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

    settings_pack.command() =  "MediapathsAudio";
    settings_pack.result()  =  QStringList{"/home/abc/audio"};

    expectSingleQueryAboutCall(*mocks_.settings_ , settings_pack);
    expectSingleQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);

    testQueryAboutCall(storage_pack);
}

TEST_F(BackendTEST , ImageSearch)
{
    QueryAboutPackage storage_pack;

    storage_pack.command()       = "SearchImages";
    storage_pack.result()        = QStringList{"/home/abc/img/image.jpg"};
    storage_pack.callArguments() = QStringList{"image.jpg"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "MediapathsImages";
    settings_pack.result()  =  QStringList{"/home/abc/img"};
    
    expectSingleQueryAboutCall(*mocks_.data_storage_ , storage_pack , settings_pack);
    expectSingleQueryAboutCall(*mocks_.settings_ , settings_pack);

    testQueryAboutCall(storage_pack);
}

TEST_F(BackendTEST , UnsupportedCommand)
{
    QueryAboutPackage pack;

    pack.command() = "XYZ";
    pack.result()  = QStringList{"WrongCmd"};

    testQueryAboutCall(pack);
}

TEST_F(BackendTEST , MultipleCall)
{
    QueryAboutPackage storage_pack;

    int calls_count{10};

    storage_pack.command()       = "SearchImages";
    storage_pack.result()        = QStringList{"/home/abc/img/image.jpg"};
    storage_pack.callArguments() = QStringList{"image.jpg"};

    QueryAboutPackage settings_pack;

    settings_pack.command() =  "MediapathsImages";
    settings_pack.result()  =  QStringList{"/home/abc/img"};
    
    expectQueryAboutCall(*mocks_.data_storage_ , calls_count ,storage_pack , settings_pack);
    expectQueryAboutCall(*mocks_.settings_ , calls_count , settings_pack);

    testQueryAboutCall(storage_pack, calls_count);
}


//TEST IDEA MULTIPLE BACKEND CALLS , MIX CALLS

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


