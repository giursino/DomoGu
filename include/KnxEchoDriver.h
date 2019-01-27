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

#ifndef KNXECHODRIVER_H
#define KNXECHODRIVER_H

#include "KnxDriver.h"
#include <mutex>
#include <condition_variable>
#include <queue>

class KnxEchoDriver : public KnxDriver
{
public:
    KnxEchoDriver();

    bool init();
    bool deinit();

    bool read(KnxMessage &message);
    bool write(const KnxMessage &message);

private:
    std::queue<KnxMessage> m_buffer;

    std::mutex m_lock;

    std::condition_variable m_not_empty;
    std::condition_variable m_not_full;
};

#endif // KNXECHODRIVER_H
