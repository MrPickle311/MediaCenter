#include "Backend.hpp"

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

IProxy::IProxy(QObject *parent):
    QObject(parent)
{}

QVariant Backend::queryAbout(QString sender, QString what)
{
    
}
