#ifndef TAPKOTINYSERIAL_H
#define TAPKOTINYSERIAL_H

#include "knxdriver.h"

class KnxTinySerial : public KnxDriver
{
public:
    KnxTinySerial();

    // KnxDriver interface
public:
    bool read(KnxMessage &message);
    bool write(const KnxMessage message);
};

#endif // TAPKOTINYSERIAL_H