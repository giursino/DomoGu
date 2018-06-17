#ifndef KNXINTERFACE_H
#define KNXINTERFACE_H


class KnxInterface
{
public:
    KnxInterface();

    virtual void OnMessageReceived() = 0;
};

#endif // KNXINTERFACE_H
