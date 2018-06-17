#ifndef KNXFAKEDRIVER_H
#define KNXFAKEDRIVER_H

#include "knxdriver.h"

class KnxFakeDriver : public KnxDriver
{
public:
    KnxFakeDriver();

    bool read(KnxMessage &message);
    bool write(const KnxMessage &message);
};

#endif // KNXFAKEDRIVER_H
