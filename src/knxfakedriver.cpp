#include "knxfakedriver.h"
#include "log.h"

using namespace log;

KnxFakeDriver::KnxFakeDriver()
{

}

bool KnxFakeDriver::read(KnxMessage &message)
{
    FILE_LOG(logERROR) << "Method not implemented";
}

bool KnxFakeDriver::write(const KnxMessage message)
{
    FILE_LOG(logERROR) << "Method not implemented";
}
