#include "knxfakedriver.h"
#include "log.h"

using namespace log;

KnxFakeDriver::KnxFakeDriver()
{
    //m_buffer = KnxMessage({0xDE, 0xAD, 0xBE, 0xEF});
}

bool KnxFakeDriver::read(KnxMessage &message)
{
    FILE_LOG(logWARNING) << "Fake method";
    //message = m_buffer;
    return true;
}

bool KnxFakeDriver::write(const KnxMessage &message)
{
    FILE_LOG(logWARNING) << "Fake method";
    FILE_LOG(logWARNING) << "Writing to internal buffer: " << message.get_string();
    //m_buffer = message;
    return true;
}
