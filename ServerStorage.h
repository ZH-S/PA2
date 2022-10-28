//
// Created by work on 10/24/22.
//

#ifndef LAPTOP_FACTORY_SRC_SERVERSTORAGE_H
#define LAPTOP_FACTORY_SRC_SERVERSTORAGE_H

#include "map"
#include "vector"
#include "mutex"
#include "Messages.h"
#include "RWLock.h"

using namespace std;


class ServerStorage {
private:
    ReadWriteLock lock;
    std::map<int, int> customer_record;
    std::vector<MapOp> state_machine_log;

public:
    ServerStorage() = default;

    void place_order(int customer_id, int latest_number_of_customer);

    int read_record(int customer_id);

    std::vector<MapOp> get_log() { return state_machine_log; }

    int getIdx() {
        return state_machine_log.size() - 1;
    }
};


#endif //LAPTOP_FACTORY_SRC_SERVERSTORAGE_H
