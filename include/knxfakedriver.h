#ifndef KNXFAKEDRIVER_H
#define KNXFAKEDRIVER_H

#include "knxdriver.h"

class KnxFakeDriver : public KnxDriver
{
public:
    KnxFakeDriver();

    // KnxDriver interface
public:
    bool read(KnxMessage &message);
    bool write(const KnxMessage message);
};

#endif // KNXFAKEDRIVER_H
