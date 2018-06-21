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

#include "KnxEchoDriver.h"

#include "log.h"

using namespace log;

KnxEchoDriver::KnxEchoDriver():
    m_buffer()
{

}

bool KnxEchoDriver::init()
{
    FILE_LOG(logINFO) << "Init KNX Echo Driver";
}

bool KnxEchoDriver::deinit()
{
    FILE_LOG(logINFO) << "Deinit KNX Echo Driver";
}

bool KnxEchoDriver::read(KnxMessage &message)
{
    FILE_LOG(logINFO) << "Waiting for reading...";

    std::unique_lock<std::mutex> lock(m_lock);
    m_not_empty.wait(lock, [this]{return !m_buffer.empty();});

    message = m_buffer.front();
    m_buffer.pop();

    FILE_LOG(logINFO) << "Read: " << message.get_string();

    lock.unlock();
    m_not_full.notify_one();

    return true;
}

bool KnxEchoDriver::write(const KnxMessage &message)
{
    FILE_LOG(logINFO) << "Waiting for writing...";

    std::unique_lock<std::mutex> lock(m_lock);
    m_not_full.wait(lock, [this]{return true;});

    m_buffer.push(message);

    FILE_LOG(logINFO) << "Written: " << message.get_string();

    lock.unlock();
    m_not_empty.notify_one();

    return true;
}
