#include "BackendTests.hpp"
#include <QCoreApplication>

UIMock::UIMock(std::shared_ptr<Backend> backend, QObject *parent) :
    QObject(parent),
    backend_(backend)
{
    //QObject::connect(this , &UIMock::requestAction , backend_.get() , &Backend::requestAction );
}

QVariant UIMock::queryAbout(QString sender, QString what)
{
    emit dataReady();
    return backend_->queryAbout(sender,what);
}


BackendTEST::BackendTEST():
    ui_mock_{backend_},
    backend_{nullptr}
{
    //BackendBuilder builder;
//
    //builder.setDataBackendDependency(&data_backend_mock_);
    //builder.setEnvironmentDependency(&env_backend_mock_);
    //builder.setSettingsDependency(&settings_backend_mock_);
    //builder.setMaxThreadsCount(3);
//
    //backend_ = std::move(builder.getBackendObject());

    QObject::connect(&loop_,&DelayedEventLoop::runned, &wrapper_ , &QFunctionWrapper::invoke);
}

TEST_F(BackendTEST, InitialTest)
{
    //connect this
    std::function<void()> ui_action = [&]()
    {
        //waits until searched QStringList is prepared
        emit ui_mock_.requestAction("Audio","WaitSearchSongs",{QString{"song3.mp3"}});
    };

    wrapper_.setFunction(std::move(ui_action));
    QObject::connect(&ui_mock_ , &UIMock::dataReady , &loop_ , &DelayedEventLoop::killTestEventLoop);

    loop_.startTestEventLoop();

    QStringList result {ui_mock_.queryAbout("Audio","FoundSongs").toStringList()};

    EXPECT_EQ(result.size(),1);
    EXPECT_STREQ(result[0].toStdString().c_str(),"song3.mp3");
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc , argv};
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


