#ifndef KNXMANAGER_H
#define KNXMANAGER_H

#include "knxdriver.h"
#include "knxmessage.h"
#include "knxclientinterface.h"
#include <set>

class KnxManager
{
public:
    KnxManager(KnxDriver::DriverType driver);
    ~KnxManager();

    void Loop();

    bool Register(const KnxClientInterface *client);
    bool Deregister(const KnxClientInterface *client);

    void SendMessage(const KnxMessage &message);

private:
    KnxDriver* m_driver;
    std::set<KnxClientInterface *> m_clients;
};

#endif // KNXMANAGER_H
