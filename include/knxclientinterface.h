#ifndef KNXCLIENTINTERFACE_H
#define KNXCLIENTINTERFACE_H


class KnxClientInterface
{
public:
    KnxClientInterface();

    virtual void OnMessageReceived() = 0;
};

#endif // KNXCLIENTINTERFACE_H
