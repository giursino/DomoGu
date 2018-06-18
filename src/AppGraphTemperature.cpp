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

#include "AppGraphTemperature.h"
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

void AppGraphTemperature::OnMessageReceived(KnxMessage &message)
{
    FILE_LOG(logINFO) << "Received message: " << message.get_string();
}
