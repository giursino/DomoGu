#include "knxdriver.h"
#include "knxtinyserial.h"
#include "knxfakedriver.h"

KnxDriver* KnxDriver::create_knx_driver(KnxDriver::DriverType interface)
{
    if (interface == KnxDriver::KNX_TINY_SERIAL) {
        return new KnxTinySerial();
    }
    else {
        return new KnxFakeDriver();
    }
}

bool KnxDriver::init()
{

}
