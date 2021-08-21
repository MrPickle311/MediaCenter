#pragma once

#include <Backend.hpp>
#include <gmock/gmock.h>

class BackendMediatorMOCK: public IMediator
{
    MOCK_METHOD3(requestAction , void(QString,QString,QVariantList) );
    MOCK_METHOD2(queryAbout , QVariant(QString , QString) );
};

//virtual void requestAction(QString sender,QString requestedAction,QVariantList args = {}) = 0;
//virtual QVariant queryAbout(QString sender, QString what) = 0;

// FRONTEND MOCK
