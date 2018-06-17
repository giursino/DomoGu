#ifndef KNXMANAGER_H
#define KNXMANAGER_H

#include "knxdriver.h"
#include "knxmessage.h"
#include "knxinterface.h"

class KnxManager
{
public:
    KnxManager(KnxDriver::DriverType driver);
    ~KnxManager();

    void Loop();

    bool Register(const KnxInterface *client);
    bool Deregister(const KnxInterface *client);

    void SendMessage(const KnxMessage &message);

private:
    KnxDriver* m_driver;
};

#endif // KNXMANAGER_H
