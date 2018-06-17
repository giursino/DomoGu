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

    /*KnxManager* knxManager = new KnxManager::KnxManager(KnxDriver::KNX_TINY_SERIAL);
    AppGraphTemperature* appTemperature = new AppGraphTemperature::AppGraphTemperature();

    knxManager->Register(appTemperature);


    knxManager->Loop();

*/
    return 0;
}
