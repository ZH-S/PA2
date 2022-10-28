#include <cstring>
#include <utility>
#include "ClientStub.h"

ClientStub::ClientStub() {}

int ClientStub::Init(std::string Ip, int Port) {
    this->ip = std::move(Ip);
    this->port = Port;
    return socket.Init(ip, port);
}

int ClientStub::Reconnect() {
    return socket.Init(ip, port);
}

void ClientStub::SendIdentification(int stat) {
    char buf[256];
    int status = stat;
    std::memcpy(buf, &status, sizeof(status));
    socket.Send(buf, sizeof(status), 0);
}

LaptopInfo ClientStub::OrderLaptop(LaptopOrder order) {
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


void ClientStub::startReplication(ReplicationRequest request) {
    LaptopOrder ord;
    char buf[128];
    request.Marshal(buf);
    if (socket.Send(buf, request.Size(), 0)) {
        if (socket.Recv(buf, ord.Size(), 0)) {
            ord.Unmarshal(buf);
        }
    }
}