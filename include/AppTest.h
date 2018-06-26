#ifndef APPTEST_H
#define APPTEST_H

#include "KnxClientInterface.h"
#include "KnxManager.h"
#include <thread>

class AppTest : public KnxClientInterface
{
public:
    AppTest(KnxManager *knx);
    ~AppTest();

    void Loop();

    void OnMessageReceived(KnxMessage &message) const;

private:
    KnxManager* m_knx;
    std::thread m_thread;
};

#endif // APPTEST_H
