#pragma once

#include <Backend.hpp>
#include <gmock/gmock.h>

class BackendMediatorMOCK: public IMediator
{
public:
    MOCK_METHOD3(requestAction , void(QString,QString,QVariantList) );
    MOCK_METHOD2(queryAbout , QVariant(QString , QString) );
};

class GeneralBackendTEST : public ::testing::Test
{
public:
    GeneralBackendTEST();
protected:
    GeneralBackend gb_;
    BackendMediatorMOCK data_backend_mock_;
protected:
};


// FRONTEND MOCK
