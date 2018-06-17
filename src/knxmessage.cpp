#include "knxmessage.h"
#include <sstream>
#include <iomanip>

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
    std::stringstream ssMessage;
    for (auto byte=m_message.begin(); byte!=m_message.end(); byte++) {
        ssMessage << "0x" << std::hex << std::to_string(*byte) << " ";
    }
    return ssMessage.str();
}
