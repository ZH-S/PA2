#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"
#include "ServerStorage.h"
#include "ClientStub.h"


LaptopInfo LaptopFactory::CreateRegularLaptop(LaptopOrder order, int engineer_id) {
    LaptopInfo laptop;
    laptop.CopyOrder(order);
    laptop.SetEngineerId(engineer_id);

    std::promise<CustomerRecord> prom;
    std::future<CustomerRecord> fut = prom.get_future();

    std::unique_ptr<AdminRequest> req = std::unique_ptr<AdminRequest>(new AdminRequest);
    req->type = 1;
    req->ops = {1, order.GetCustomerId(), order.GetOrderNumber()};
    req->promise = std::move(prom);


    erq_lock.lock();
    erq.push(std::move(req));
    erq_cv.notify_one();
    erq_lock.unlock();

    CustomerRecord record = fut.get();
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
    ReplicationRequest req;
    CustomerRecord record;
    ServerStub stub;
    int laptop_type;
    stub.Init(std::move(socket));
    int status = stub.ReceiveStatusInfo();
    std::cout << "status " << status << std::endl;

    while (true) {
        if (status == 2) { // IFA operation
            req = stub.ReceiveReplicationRequest();
            if (!stub.isValid()) {
                this->primary_id = -1;
                return;
            }
            this->primary_id = req.getFactoryId();
            this->committed_index = req.getCommittedIndex();
            this->last_index = req.getLastIndex();
            int cid = req.getOperation().arg1;
            int last_ordn = req.getOperation().arg2;
            server_storage.place_order(cid, last_ordn);
            LaptopOrder ord;
            ord.SetOrder(cid, last_ordn, 4);
            stub.SendReplicationResult(ord);
            std::cout << "success in replicate " << req << std::endl;
        } else if (status == 1) { // Engineer operation
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
            if (this->factory_id != this->primary_id || this->primary_id == -1) {
                startConnection();
            }
            this->last_index = server_storage.getIdx();
            ReplicationRequest request = ReplicationRequest(this->factory_id, this->committed_index,
                                                            this->last_index,
                                                            {1, ops.arg1, ops.arg2});
            startReplication(request);
            server_storage.place_order(ops.arg1, ops.arg2);
            this->committed_index = this->last_index;
            req->promise.set_value(req->record);
        } else {
            int last_order = server_storage.read_record(req->record.getCustomerId());
            req->record.setLastOrder(last_order);
            req->promise.set_value(req->record);
        }
    }
}

void LaptopFactory::startConnection() {
    this->primary_id = this->factory_id;
    for (int i = 0; i < this->peer_list.size(); i++) {
        Peer p = this->peer_list[i];
        std::unique_ptr<ClientStub> stub = std::unique_ptr<ClientStub>(new ClientStub);
        stub->Init(p.ip_addr, p.port);
        if (stub->isValid()) {
            stub->SendIdentification(2);
        }
        stubs.push_back(std::move(stub));
    }
}

void LaptopFactory::startReplication(ReplicationRequest request) {
    for (auto it = stubs.begin(); it != stubs.end(); ++it) {
        auto stub = it->get();
        if (!stub->isValid()) {
            stub->Reconnect();
            if (!stub->isValid()) {
                continue;
            }
            stub->SendIdentification(2);
        }
        stub->startReplication(request);
    }
}

void LaptopFactory::startRecovery() {

}