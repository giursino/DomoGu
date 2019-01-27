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

#include "KnxTinySerial.h"
#include "log.h"

using namespace log;

KnxTinySerial::KnxTinySerial():
  m_serial_port_name("/dev/serial")
{

}

bool KnxTinySerial::init()
{
    FILE_LOG(logINFO) << "KnxTinySerial init function";
    //m_serial_port = new SerialPort(m_serial_port_name);
    //m_serial_port->Open();
    //m_serial_port->SetBaudRate(SerialPort::BaudRate::BAUD_19200);
    return true;
}

bool KnxTinySerial::deinit()
{
    FILE_LOG(logINFO) << "KnxTinySerial deinit function";
    m_serial_port->Close();
    delete(m_serial_port);
    m_serial_port=nullptr;
    return true;
}

bool KnxTinySerial::read(KnxMessage &message)
{
    return false;
}

bool KnxTinySerial::write(const KnxMessage &message)
{
    return false;
}
