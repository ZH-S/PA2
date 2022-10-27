#include <cstring>
#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
    this->socket = std::move(socket);
}

int ServerStub::ReceiveStatusInfo() {
    char buffer[32];
    int status_code;
    if (socket->Recv(buffer, sizeof(status_code), 0)) {
        memcpy(&status_code, buffer, sizeof(status_code));
    }
    return status_code;
}

LaptopOrder ServerStub::ReceiveOrderRequest() {
    char buffer[128];
    LaptopOrder order;
    if (socket->Recv(buffer, order.Size(), 0)) {
        order.Unmarshal(buffer);
    }
    return order;
}

ReplicationRequest ServerStub::ReceiveReplicationRequest() {
    char buffer[128];
    ReplicationRequest request;
    if (socket->Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
    }
    return request;
}

/**RequestType ServerStub::Receive() {
    char buffer[128];
    int type = -1;
    LaptopOrder order;
    ReplicationRequest request;
    if (socket->Recv(buffer, order.Size(), 0)) {
        type = request.Unmarshal(buffer);
        if (type != -2) {
            type = -1;
            order.Unmarshal(buffer);
        } else {
            char buf[128];
            int size = request.Size() - order.Size();
            socket->Recv(buf, size, 0);
            std::memcpy(buffer + order.Size(), buf, size);
        }
    }
    RequestType y;
    y.type = type;
    y.request = request;
    y.order = order;
    return y;
}*/

int ServerStub::SendLaptop(LaptopInfo info) {
    char buffer[32];
    info.Marshal(buffer);
    return socket->Send(buffer, info.Size(), 0);
}

int ServerStub::ReturnRecord(CustomerRecord record) {
    char buffer[32];
    record.Marshal(buffer);
    return socket->Send(buffer, record.Size(), 0);
}

int ServerStub::SendReplicationResult(LaptopOrder order) {
    char buffer[32];
    order.Marshal(buffer);
    return socket->Send(buffer, order.Size(), 0);
}
