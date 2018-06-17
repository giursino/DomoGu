#include "knxmanager.h"
#include "log.h"

using namespace log;

KnxManager::KnxManager(KnxDriver::DriverType driver)
{
    m_driver = KnxDriver::create_knx_driver(driver);
}

KnxManager::~KnxManager()
{
    KnxManager::m_clients.clear();

    if (m_driver) {
        m_driver->deinit();
        delete m_driver;
    }
}

void KnxManager::Loop()
{
    // TODO test
    for (std::set<KnxClientInterface *>::iterator it=m_clients.begin(); it!=m_clients.end(); ++it) {
        (*it)->OnMessageReceived();
    }

}

bool KnxManager::Register(KnxClientInterface *client)
{
    if (KnxManager::m_clients.insert(client).second) {
        FILE_LOG(logDEBUG) << "Client registered";
        return true;
    }
    else {
        FILE_LOG(logWARNING) << "Client already registered";
        return false;
    }
}

bool KnxManager::Deregister(KnxClientInterface *client)
{
    if (KnxManager::m_clients.erase(client) == 1) {
        FILE_LOG(logDEBUG) << "Client deregistered";
        return true;
    }
    else {
        FILE_LOG(logWARNING) << "Client not registered";
        return false;
    }
}

bool KnxManager::SendMessage(const KnxMessage &message) const
{
    return m_driver->write(message);
}
