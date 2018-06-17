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
