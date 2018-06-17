#include "appgraphtemperature.h"
#include "log.h"

using namespace log;

AppGraphTemperature::AppGraphTemperature(const KnxManager *knx)
{
    FILE_LOG(logINFO) << "AppGraphTemperature loaded";
    m_knx = knx;
}

AppGraphTemperature::~AppGraphTemperature()
{
    FILE_LOG(logINFO) << "AppGraphTemperature unloaded";
}

void AppGraphTemperature::Loop()
{
    KnxMessage msg({0x11, 0x22, 0x33});
    m_knx->SendMessage(msg);

}

void AppGraphTemperature::OnMessageReceived()
{
    FILE_LOG(logINFO) << "Received message";
}
