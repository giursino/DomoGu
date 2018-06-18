/*
Copyright 2018 giursino

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "KnxManager.h"
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
