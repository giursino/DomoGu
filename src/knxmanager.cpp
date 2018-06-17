#include "knxmanager.h"
#include "log.h"

using namespace log;

KnxManager::KnxManager(KnxDriver::DriverType driver)
{
    m_driver = KnxDriver::create_knx_driver(driver);
}

KnxManager::~KnxManager()
{
    if (m_driver) {
        m_driver->deinit();
        delete m_driver;
    }
}

void KnxManager::Loop()
{

}

bool KnxManager::Register(const KnxClientInterface *client)
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

bool KnxManager::Deregister(const KnxClientInterface *client)
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

void KnxManager::SendMessage(const KnxMessage &message)
{
    return m_driver->write(message);
}
