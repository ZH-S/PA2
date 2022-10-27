#include <iostream>
#include <memory>
#include <cstring>

#include "ServerThread.h"
#include "ServerStub.h"
#include "ServerStorage.h"


LaptopInfo LaptopFactory::CreateRegularLaptop(LaptopOrder order, int engineer_id) {
    LaptopInfo laptop;
    laptop.CopyOrder(order);
    laptop.SetEngineerId(engineer_id);
    laptop.SetAdminId(1);

    std::unique_ptr<AdminRequest> req = std::unique_ptr<AdminRequest>(new AdminRequest);
    req->type = 1;
    req->ops = {1, order.GetCustomerId(), order.GetOrderNumber()};

    erq_lock.lock();
    erq.push(std::move(req));
    erq_cv.notify_one();
    erq_lock.unlock();

    return laptop;
}

CustomerRecord LaptopFactory::GetCustomerRecord(LaptopOrder order, int engineer_id) {
    int record = server_storage.read_record(order.GetCustomerId());
    CustomerRecord record1 = CustomerRecord(order.GetCustomerId(), record);
    return record1;
}

void LaptopFactory::EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
    int engineer_id = id;
    LaptopOrder order;
    LaptopInfo laptop;
    CustomerRecord record;
    ServerStub stub;
    int laptop_type;
    stub.Init(std::move(socket));

    while (true) {
        int status = stub.ReceiveStatusInfo();
        std::cout << "status " << status << std::endl;
        if (status == 2) { // IFA operation
            ReplicationRequest req = stub.ReceiveReplicationRequest();
            this->primary_id = req.getFactoryId();
            this->committed_index = req.getCommittedIndex();
            this->last_index = req.getLastIndex();
            int cid = req.getOperation().arg1;
            int last_ordn = req.getOperation().arg2;
            server_storage.place_order(cid, last_ordn);
            LaptopOrder ord;
            ord.SetOrder(cid, last_ordn, 4);
            stub.SendReplicationResult(ord);
            std::cout << "success in replicate cid:" << cid << " ,oid" << last_ordn << std::endl;
        } else { // Engineer operation
            order = stub.ReceiveOrderRequest();
            if (!order.IsValid()) {
                return;
            }
            laptop_type = order.GetLaptopType();
            switch (laptop_type) {
                case 1:
                    laptop = CreateRegularLaptop(order, engineer_id);
                    stub.SendLaptop(laptop);
                    break;
                case 2:
                case 3:
                    record = GetCustomerRecord(order, engineer_id);
                    stub.ReturnRecord(record);
                    break;
                default:
                    std::cout << "Undefined request type: " << laptop_type << std::endl;
            }
        }
    }
}

void LaptopFactory::ProductionAdminThread() {
    std::unique_lock<std::mutex> ul(erq_lock, std::defer_lock);
    while (true) {
        ul.lock();
        if (erq.empty()) {
            erq_cv.wait(ul, [this] { return !erq.empty(); });
        }
        auto req = std::move(erq.front());
        erq.pop();

        ul.unlock();

        // fill in record
        if (req->type == 1) {
            MapOp ops = req->ops;
            server_storage.place_order(ops.arg1, ops.arg2);
            std::cout << "ops 1:" << ops.arg1 << ", ops 2: " << ops.arg2 << std::endl;
        } else {
            int last_order = server_storage.read_record(req->record.getCustomerId());
            req->record.setLastOrder(last_order);
            req->promise.set_value(req->record);
        }

        std::mutex mtx;
        // replication
        mtx.lock();
        if (this->factory_id != this->primary_id || this->primary_id == -1) {
            startConnection(); //TODO need to handle the exception by failure
        }
        this->last_index = server_storage.getIdx();
        ReplicationRequest request = ReplicationRequest(this->factory_id, this->committed_index, this->last_index,
                                                        {1, req->ops.arg1, req->ops.arg2});
        startReplication(request);
        mtx.unlock();
    }
}

// start connection as primary
void LaptopFactory::startConnection() {
    this->primary_id = this->factory_id;
    for (int i = 0; i < this->peer_list.size(); i++) {
        Peer p = this->peer_list[i];
        ClientSocket sock;
        sock.Init(p.ip_addr, p.port);
        this->socket_list[p.server_id] = sock;
    }
}

void LaptopFactory::startReplication(ReplicationRequest request) {
    std::cout << "replication success" << std::endl;
    LaptopOrder ord;
    for (auto it = socket_list.begin(); it != socket_list.end(); ++it) {
        ClientSocket sock = it->second;
        int status = 2;
        char buffer[16];
        std::cout << "buffer " << buffer << std::endl;
        std::memcpy(buffer, &status, sizeof(status));
        sock.Send(buffer, sizeof(status), 0);

        char buf[128];
        request.Marshal(buf);
        if (sock.Send(buf, request.Size(), 0)) {
            if (sock.Recv(buf, ord.Size(), 0)) {
                ord.Unmarshal(buf);
            }
        }
    }
    std::cout << "replication success" << std::endl;
}



