#ifndef APPGRAPHTEMPERATURE_H
#define APPGRAPHTEMPERATURE_H

#include "knxclientinterface.h"

class AppGraphTemperature : public KnxClientInterface
{
public:
    AppGraphTemperature();
    ~AppGraphTemperature();

    void OnMessageReceived();
};

#endif // APPGRAPHTEMPERATURE_H
