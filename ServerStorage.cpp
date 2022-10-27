//
// Created by work on 10/24/22.
//

#include "ServerStorage.h"

void ServerStorage::place_order(int customer_id, int latest_number_of_customer) {
    MapOp op = MapOp{1, customer_id, latest_number_of_customer};
    lock.writeLock();
    state_machine_log.push_back(op);
    customer_record[customer_id] = latest_number_of_customer;
    lock.writeUnlock();
}

int ServerStorage::read_record(int customer_id) {
    lock.readLock();
    if (!customer_record.count(customer_id)) return -1;
    int rec = customer_record[customer_id];
    lock.readUnlock();
    return rec;
}