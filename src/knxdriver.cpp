#include "knxdriver.h"

using namespace KnxDriver;

KnxDriver* create_knx_interface(DriverType interface)
{
    if (interface == DriverType::TAPKO_KNX_TINY_SERIAL) {

    }
    else {
        return new KnxVoidInterface;
    }
}
