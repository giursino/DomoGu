#include "appgraphtemperature.h"
#include "log.h"

using namespace log;

AppGraphTemperature::AppGraphTemperature()
{
    FILE_LOG(logINFO) << "AppGraphTemperature loaded";
}

AppGraphTemperature::~AppGraphTemperature()
{
    FILE_LOG(logINFO) << "AppGraphTemperature unloaded";
}

void AppGraphTemperature::OnMessageReceived()
{
    FILE_LOG(logINFO) << "Received message";
}
