#ifndef KNXFAKEDRIVER_H
#define KNXFAKEDRIVER_H

#include "knxdriver.h"

class KnxMessage;

class KnxFakeDriver : public KnxDriver
{
public:
    KnxFakeDriver();

    bool read(KnxMessage &message);
    bool write(const KnxMessage &message);

private:
    //KnxMessage m_buffer;
};

#endif // KNXFAKEDRIVER_H
