/*
Copyright 2018 giursino

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

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
