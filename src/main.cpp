#include "knxmanager.h"
#include "appgraphtemperature.h"
#include "log.h"
#include <iostream>

using namespace std;
using namespace log;

int main()
{
    FILELog::ReportingLevel() = logTRACE;

    FILE_LOG(logDEBUG) << "Starting DomoGu...";

    KnxManager *knxManager = new KnxManager(KnxDriver::FAKE_DRIVER);
    AppGraphTemperature* appTemperature1 = new AppGraphTemperature(knxManager);
    AppGraphTemperature* appTemperature2 = new AppGraphTemperature(knxManager);

    knxManager->Register(appTemperature1);
    knxManager->Register(appTemperature2);

    knxManager->Loop();
    knxManager->Register(appTemperature1);
    knxManager->Deregister(appTemperature2);

    knxManager->Register(appTemperature2);
    knxManager->Deregister(appTemperature1);

    appTemperature1->Loop();

    return 0;
}
