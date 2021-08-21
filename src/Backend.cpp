#include "Backend.hpp"

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

IProxy::IProxy(QObject *parent):
    QObject(parent)
{}

void Backend::requestAction(QString sender, QString requestedAction, QVariantList args)
{
    
}

QVariant Backend::queryAbout(QString sender, QString what)
{
    
}
