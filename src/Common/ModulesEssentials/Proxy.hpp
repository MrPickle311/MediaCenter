#pragma once
#include "Mediator.hpp"

class IProxy : public IMediator
{
    Q_OBJECT
public:
    explicit IProxy(QObject *parent = nullptr);
};

using IProxyPtr =  std::shared_ptr<IProxy>;