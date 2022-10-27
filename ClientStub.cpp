#include <cstring>
#include "ClientStub.h"

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
    return socket.Init(ip, port);
}

void ClientStub::SendIdentification() {
    char buf[256];
    int status = 1;
    std::memcpy(buf, &status, sizeof(status));
    socket.Send(buf, sizeof(status), 0);
}

LaptopInfo ClientStub::OrderLaptop(LaptopOrder order) {
    SendIdentification();
    LaptopInfo info;
    char buffer[32];
    int size;
    order.Marshal(buffer);
    size = order.Size();
    if (socket.Send(buffer, size, 0)) {
        size = info.Size();
        if (socket.Recv(buffer, size, 0)) {
            info.Unmarshal(buffer);
        }
    }
    return info;
}

CustomerRecord ClientStub::ReadRecord(LaptopOrder request) {
    SendIdentification();
    CustomerRecord record;
    char buffer[32];
    int size;
    request.Marshal(buffer);
    size = request.Size();
    if (socket.Send(buffer, size, 0)) {
        size = record.Size();
        if (socket.Recv(buffer, size, 0)) {
            record.Unmarshal(buffer);
        }
    }
    return record;
}

