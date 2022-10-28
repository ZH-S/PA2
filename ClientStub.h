#ifndef __CLIENT_STUB_H__
#define __CLIENT_STUB_H__

#include <string>

#include "ClientSocket.h"
#include "Messages.h"

class ClientStub {
private:
    std::string ip;
    int port;
    ClientSocket socket;
public:
    ClientStub();

    ~ClientStub() {
        socket.Close();
    }

    int Init(std::string ip, int port);

    LaptopInfo Order(LaptopOrder order);

    CustomerRecord ReadRecord(LaptopOrder request);

    void SendIdentification(int stat);

    void startReplication(ReplicationRequest request);

    bool isValid() { return socket.isValid(); }

    int Reconnect();
};


#endif // end of #ifndef __CLIENT_STUB_H__
