#include "Mediator.hpp"
#include <iostream>

IMediator::IMediator(QObject *parent):
    QObject(parent)
{}

QStringList Mediator::queryAbout(const QString& command, QStringList args) 
{
    std::cout << __FILE__ << " " << __LINE__  << " Not implemented!\n";
}
