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

#include "AppTest.h"

#include "log.h"
#include <chrono>
#include <cstdint>

using namespace log;

AppTest::AppTest(KnxManager *knx):
  m_knx(knx)
{
  FILE_LOG(logINFO) << "AppTest loaded";

  m_thread = std::thread([this] {Loop();});
}

AppTest::~AppTest()
{
    m_thread.join();
    m_knx->Deregister(this);
    FILE_LOG(logINFO) << "AppTest unloaded";
}

void AppTest::Loop()
{
    uint8_t count=0;
    FILE_LOG(logINFO) << "Registering...";
    m_knx->Register(this);

    FILE_LOG(logINFO) << "Starting loop...";
    while(true) {
        count++;
        KnxMessage msg({0xAA, 0xBB, count});
        m_knx->SendMessage(msg);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void AppTest::OnMessageReceived(KnxMessage &message) const
{
    FILE_LOG(logINFO) << "Nothing to do!";
}
