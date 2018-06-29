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
    m_message()
{
}

KnxMessage::KnxMessage(const std::vector<uint8_t> message):
    m_message(message)
{

}

bool KnxMessage::set_raw(const std::vector<std::uint8_t> message)
{
    m_message = message;
    return true;
}

bool KnxMessage::get_raw(std::vector<uint8_t> &message) const
{
    message = m_message;
    return true;
}

std::string KnxMessage::get_string() const
{
    std::stringstream ssMessage;
    for (uint8_t byte : m_message) {
        ssMessage << "0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << int(byte) << " ";
    }
    return ssMessage.str();
}

bool KnxMessage::get_src(KnxAddr &addr) const
{
    if (!is_message_valid()) {
        return false;
    }

    addr.set_value((m_message[1]<<8) + m_message[2]);
    return true;
}

bool KnxMessage::get_dest(KnxAddr &addr) const
{
    if (!is_message_valid()) {
        return false;
    }

    addr.set_value((m_message[3]<<8) + m_message[4]);
    return true;
}

bool KnxMessage::get_frame_type(FrameType &value) const
{
    if (!is_message_valid()) {
        return false;
    }

    if ((m_message[0] & 0x53) == 0x10) {
        // L_Data-Frame
        if ((m_message[0] & 0x80) == 0x10) {
            value=FrameType::L_Data_Standard;
        }
        else{
            value=FrameType::L_Data_Extended;
        }
        return true;
    }
    else if (m_message[0] == 0xF0) {
        value=FrameType::L_Poll_Data;
        return true;
    }
    else if (m_message[0] == 0xCC) {
        value=FrameType::ACK;
        return true;
    }
    else if (m_message[0] == 0x0C) {
        value=FrameType::NAK;
        return true;
    }
    else if (m_message[0] == 0xC0) {
        value=FrameType::BUSY;
        return true;
    }
    else if (m_message[0] == 0x00) {
        value=FrameType::NAK_and_BUSY;
        return true;
    }
    return false;
}

bool KnxMessage::get_priority(Priority &value) const
{
    if (!is_message_valid()) {
        return false;
    }

    FrameType frame;
    if (!get_frame_type(frame)) {
        return false;
    }

    if ((frame != FrameType::L_Data_Standard) && (frame != FrameType::L_Data_Extended)) {
        return false;
    }

    switch (m_message[0] & 0x0C) {
    case 0x00:
        value = Priority::system;
        break;
    case 0x08:
        value = Priority::urgent;
        break;
    case 0x04:
        value = Priority::normal;
        break;
    case 0x0C:
        value = Priority::low;
        break;
    }
    return true;
}

bool KnxMessage::is_destination_broadcast_address() const
{
    return ((m_message[3] == 0x00) && (m_message[4] == 0x00));
}

bool KnxMessage::is_destination_group_address() const
{
    return ((m_message[5] & 0x80) == 0x80);
}

bool KnxMessage::get_hopcount(uint8_t &hopcount) const
{
    if (!is_message_valid()) {
        return false;
    }
    hopcount = (m_message[5] & 0x70) >> 4;
    return true;
}

bool KnxMessage::get_length(uint8_t &length) const
{
    if (!is_message_valid()) {
        return false;
    }
    length = m_message[5] & 0x0F;
    return true;
}

bool KnxMessage::get_transport_layer_services(TransportLayerServices &value) const
{
    if (!is_message_valid()) {
        return false;
    }

    if (is_destination_group_address()) {
        switch (m_message[6] & 0xFC) {
        case 0x00:
            if (is_destination_broadcast_address()) {
                value = TransportLayerServices::T_Data_Broadcast;
                return true;
            }
            else {
                value = TransportLayerServices::T_Data_Group;
                return true;
            }
            break;
        case 0x04:
            value = TransportLayerServices::T_Data_Tag_Group;
            return true;
            break;
        }
    }
    else {
        if (m_message[6] == 0x80){
            value = TransportLayerServices::T_Connect;
            return true;
        }
        else if (m_message[6] == 0x81) {
            value = TransportLayerServices::T_Disconnect;
            return true;
        }
        else if ((m_message[6] & 0xFC) == 0x00) {
            value = TransportLayerServices::T_Data_Individual;
            return true;
        }
        else if ((m_message[6] & 0xC0) == 0x40) {
            value = TransportLayerServices::T_Data_Connected;
            return true;
        }
        else if ((m_message[6] & 0xC3) == 0xC2) {
            value = TransportLayerServices::T_ACK;
            return true;
        }
        else if ((m_message[6] & 0xC3) == 0xC3) {
            value = TransportLayerServices::T_NAK;
            return true;
        }
    }
    return false;

}

bool KnxMessage::get_transport_layer_sequence_num(uint8_t &value) const
{
    if (!is_message_valid()) {
        return false;
    }
    value = ((m_message[6] & 0x3C) >> 2);
    return true;
}

bool KnxMessage::is_message_valid() const
{
    if (m_message.size() == 0) {
        return false;
    }
    // TODO: check last octect
    return true;
}

