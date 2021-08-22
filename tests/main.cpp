#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

#include <QUrl>
#include <MainBackendTests.hpp>

#include <QCoreApplication>

using ::testing::Return;

class Value {
public:

    virtual std::string getValue() = 0;

};

class MyClass {

public:

    bool isValueOk(std::shared_ptr<Value> value) {
        return value->getValue() == "Ok";
    }

};

class ValueMock : public Value {

public:

    MOCK_METHOD0(getValue, std::string());

};

TEST(MyTest, test_the_value_is_ok) {
    MyClass myClass;
    auto mockObject = std::make_shared<ValueMock>();

    EXPECT_CALL(*mockObject, getValue())
        .WillRepeatedly(
            Return("NotOK"));

    EXPECT_CALL(*mockObject, getValue())
        .WillRepeatedly(
            Return("Ok"));

    auto value = myClass.isValueOk(mockObject);

    int* i = NULL;

    std::cout << --i << '\n';

    EXPECT_TRUE(value);
}

void runAllUnitTests(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}

int main(int argc, char* argv[])
{
    //QCoreApplication app(argc, argv);

    //runAllUnitTests(argc, argv);

    //return app.exec();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


