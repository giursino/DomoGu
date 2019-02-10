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

#ifndef KNXTINYSERIALDRIVER_H
#define KNXTINYSERIALDRIVER_H

#include "KnxDriver.h"
#include <string>
#include <KnxTinySerial.h>

class KnxTinySerialDriver : public KnxDriver
{
public:
    KnxTinySerialDriver();

    bool init();
    bool deinit();
    bool read(KnxMessage &message);
    bool write(const KnxMessage &message);
private:
    static const size_t loop_ms=100;
    bool m_is_loaded;
    std::string m_serial_port_name;
    SerialPort* m_serial_port;
    KnxTinySerial* m_driver;
};

#endif // KNXTINYSERIALDRIVER_H
