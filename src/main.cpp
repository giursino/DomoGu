/*
Copyright 2018 Giuseppe Ursino

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

#include "KnxManager.h"
#include "AppGraphTemperature.h"
#include "AppTest.h"
#include "log.h"
#include <iostream>

using namespace std;
using namespace log;

int main()
{
    FILELog::ReportingLevel() = logTRACE;

    FILE_LOG(logDEBUG) << "Starting DomoGu...";

//    KnxManager *knx_manager_echo = new KnxManager(KnxDriver::DriverType::ECHO_DRIVER);
//    AppTest* app_test = new AppTest(knx_manager_echo);

    KnxManager *knx_manager = new KnxManager(KnxDriver::DriverType::KNX_TINY_SERIAL);
    AppGraphTemperature* app_temperature = new AppGraphTemperature(knx_manager);

//    delete app_test;
//    delete knx_manager_echo;

    delete app_temperature;
    delete knx_manager;

    return 0;
}
