#ifndef KNXDRIVER_H
#define KNXDRIVER_H

#include "knxmessage.h"


class KnxDriver
{
public:

    typedef enum {
        VIMAR_01847,
        KNX_TINY_SERIAL

    } DriverType;

    static KnxDriver* create_knx_driver(DriverType type);

    bool init();
    bool deinit();

    virtual bool read(KnxMessage &message) = 0;
    virtual bool write(const KnxMessage message) = 0;

};

#endif // KNXDRIVER_H
