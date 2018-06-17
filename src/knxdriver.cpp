#include "knxdriver.h"
#include "knxtinyserial.h"
#include "knxfakedriver.h"
#include "log.h"

using namespace log;

KnxDriver* KnxDriver::create_knx_driver(KnxDriver::DriverType interface)
{
    if (interface == KnxDriver::KNX_TINY_SERIAL) {
        return new KnxTinySerial();
    }
    else {
        FILE_LOG(logERROR) << "Driver not implemented";
        return new KnxFakeDriver();
    }
}

bool KnxDriver::init()
{
    FILE_LOG(logINFO) << "Base init function";
}

bool KnxDriver::deinit()
{
    FILE_LOG(logINFO) << "Base deinit function";
}
