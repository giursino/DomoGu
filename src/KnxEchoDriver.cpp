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
    m_buffer({0xDE, 0xAD, 0xBE, 0xEF}),
    m_data_ready(true)
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
    std::unique_lock<std::mutex> lk_data_ready(m_data_ready_mutex);
    m_data_ready_cv.wait(lk_data_ready, []{return m_data_ready;});
//    m_data_ready_cv.wait(lk_data_ready);

    {
      std::lock_guard<std::mutex> guard(m_data_ready_mutex);
      m_data_ready = false;
    }

    {
      std::lock_guard<std::mutex> guard(m_buffer_mutex);
      message = m_buffer;
    }

    FILE_LOG(logINFO) << "Reading: " << message.get_string();
    return true;
}

bool KnxEchoDriver::write(const KnxMessage &message)
{
    if (m_data_ready) {
        FILE_LOG(logWARNING) << "Losing previous data: " << m_buffer.get_string();
    }

    FILE_LOG(logINFO) << "Writing: " << message.get_string();
    {
      std::lock_guard<std::mutex> guard(m_buffer_mutex);
      m_buffer = message;
    }

    {
      std::lock_guard<std::mutex> guard(m_data_ready_mutex);
      m_data_ready = true;
    }

    return true;
}
