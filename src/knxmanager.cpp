#include "knxmanager.h"

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
