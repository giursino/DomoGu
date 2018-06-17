#include "knxmessage.h"

KnxMessage::KnxMessage(const std::vector<uint8_t> message)
{
    KnxMessage::m_message = message;
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
    std::string strMessage;
    for (auto byte=m_message.begin(); byte!=m_message.end(); byte++) {
        //strMessage << (*byte) << " ";
    }
    //strMessage << std::endl;
    return strMessage;
}
