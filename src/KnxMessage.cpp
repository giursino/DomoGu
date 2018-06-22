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

#include "KnxMessage.h"
#include <sstream>
#include <iomanip>

KnxMessage::KnxMessage():
    m_message({0})
{
}

KnxMessage::KnxMessage(const std::vector<uint8_t> message):
    m_message(message)
{

}

bool KnxMessage::set_raw(const std::vector<std::uint8_t> message)
{
    KnxMessage::m_message = message;
    return true;
}

bool KnxMessage::get_raw(std::vector<uint8_t> &message)
{
    message = KnxMessage::m_message;
    return true;
}

std::string KnxMessage::get_string() const
{
    std::stringstream ssMessage;
    for (uint8_t byte : m_message) {
        ssMessage << "0x" << std::setw(2) << std::setfill('0') << std::hex << int(byte) << " ";
    }
    return ssMessage.str();
}
