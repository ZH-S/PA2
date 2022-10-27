#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <list>

#include "Messages.h"
#include "ServerSocket.h"
#include "ServerStorage.h"
#include "ClientSocket.h"
#include "ServerStub.h"
#include "ClientStub.h"

struct Peer {
    int server_id;
    string ip_addr;
    int port;
};

struct AdminRequest {
    int type;
    MapOp ops;
    CustomerRecord record;
    std::promise<CustomerRecord> promise;
};


class LaptopFactory {
private:
    int last_index;
    int committed_index;
    int primary_id;
    int factory_id;

    std::vector<Peer> peer_list;
    std::list<std::unique_ptr<ClientStub>> stubs;
    ServerStorage server_storage;
    std::queue<std::unique_ptr<AdminRequest>> erq;
    std::mutex erq_lock;
    std::condition_variable erq_cv;

    void startReplication(ReplicationRequest request);

    LaptopInfo CreateRegularLaptop(LaptopOrder order, int engineer_id);

    CustomerRecord GetCustomerRecord(LaptopOrder order, int engineer_id);

    void startConnection();

public:

    void Init(int factoryId) {
        this->factory_id = factoryId;
        this->last_index = -1;
        this->committed_index = -1;
        this->primary_id = -1;
    };

    void setPeerList(vector<Peer> peerList) {
        this->peer_list = std::move(peerList);
    };

    void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);

    void ProductionAdminThread();
};

#endif // end of #ifndef __SERVERTHREAD_H__

