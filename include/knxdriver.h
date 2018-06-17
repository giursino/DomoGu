#ifndef KNXINTERFACE_H
#define KNXINTERFACE_H


class KnxDriver
{
public:
    typedef enum {
        TPUART,
        VIMAR_01847,
        TAPKO_KNX_TINY_SERIAL

    } DriverType;

    static KnxDriver* create_knx_interface(DriverType interface);


};

#endif // KNXINTERFACE_H
