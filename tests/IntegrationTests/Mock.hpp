#pragma once

#include <memory>
#include <Mediator.hpp>
#include <Proxy.hpp>
#include <gmock/gmock.h>

class MediatorMock: public IMediator
{
    Q_OBJECT;
public:
    MOCK_METHOD(QStringList , queryAbout , (const QString& , const QStringList&) );
};

using MediatorMockPtr = std::shared_ptr<MediatorMock>;

using ProxyMock = MediatorMock;

using ProxyMockPtr = MediatorMockPtr;
