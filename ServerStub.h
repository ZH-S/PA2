#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>

#include "ServerSocket.h"
#include "Messages.h"

class ServerStub {
private:
    std::unique_ptr<ServerSocket> socket;
public:
    ServerStub();

    ~ServerStub() {
        socket->Close();
    }

    void Init(std::unique_ptr<ServerSocket> socket);

    LaptopOrder ReceiveOrderRequest();

    int SendLaptop(LaptopInfo info);

    int ReturnRecord(CustomerRecord record);

    int SendReplicationResult(LaptopOrder order);

    ReplicationRequest ReceiveReplicationRequest();

    int ReceiveStatusInfo();

    bool isValid() { return socket->IsValid(); }
};

#endif // end of #ifndef __SERVER_STUB_H__
