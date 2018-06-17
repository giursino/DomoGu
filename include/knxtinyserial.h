#ifndef KNXTINYSERIAL_H
#define KNXTINYSERIAL_H

#include "knxdriver.h"

class KnxTinySerial : public KnxDriver
{
public:
    KnxTinySerial();

    bool read(KnxMessage &message);
    bool write(const KnxMessage message);
};

#endif // KNXTINYSERIAL_H
