#pragma once

#include <QObject>
#include <QVariant>

class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(QObject *parent = nullptr);
public slots:
    virtual void requestAction(QString action , QVariantList args) = 0;
    virtual QStringList requestData(QString what) = 0;
};

