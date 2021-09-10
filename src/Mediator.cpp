#include "Mediator.hpp"

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

QStringList Mediator::queryAbout(const QString& command, QStringList args) 
{
    
}
