#ifndef APPGRAPHTEMPERATURE_H
#define APPGRAPHTEMPERATURE_H

#include "knxclientinterface.h"
#include "knxmanager.h"

class AppGraphTemperature : public KnxClientInterface
{
public:
    AppGraphTemperature(const KnxManager *knx);
    ~AppGraphTemperature();

    void Loop();

    void OnMessageReceived();

private:
    const KnxManager* m_knx;
};

#endif // APPGRAPHTEMPERATURE_H
