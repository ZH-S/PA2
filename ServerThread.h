#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#include "Messages.h"
#include "ServerSocket.h"
#include "ServerStorage.h"
#include "ClientSocket.h"
#include "ServerStub.h"

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
    std::map<int, ClientSocket> socket_list;
    ServerStorage server_storage;
    std::queue<std::unique_ptr<AdminRequest>> erq;
    std::mutex erq_lock;
    std::condition_variable erq_cv;

    void startReplication(ReplicationRequest request);

    LaptopInfo CreateRegularLaptop(LaptopOrder order, int engineer_id);

    CustomerRecord GetCustomerRecord(LaptopOrder order, int engineer_id);


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

    void startConnection();

    void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);

    void ProductionAdminThread();

    void SendStatusInfo();
};

#endif // end of #ifndef __SERVERTHREAD_H__

