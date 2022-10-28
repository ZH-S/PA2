#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Messages.h"

LaptopOrder::LaptopOrder() {
    customer_id = -1;
    order_number = -1;
    laptop_type = -1;
}

void LaptopOrder::SetOrder(int id, int number, int type) {
    customer_id = id;
    order_number = number;
    laptop_type = type;
}

int LaptopOrder::GetCustomerId() { return customer_id; }

int LaptopOrder::GetOrderNumber() { return order_number; }

int LaptopOrder::GetLaptopType() { return laptop_type; }

int LaptopOrder::Size() {
    return sizeof(customer_id) + sizeof(order_number) + sizeof(laptop_type);
}

void LaptopOrder::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_order_number = htonl(order_number);
    int net_laptop_type = htonl(laptop_type);
    int offset = 0;
    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(buffer + offset, &net_laptop_type, sizeof(net_laptop_type));
}

void LaptopOrder::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_order_number;
    int net_laptop_type;
    int offset = 0;
    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(&net_laptop_type, buffer + offset, sizeof(net_laptop_type));

    customer_id = ntohl(net_customer_id);
    order_number = ntohl(net_order_number);
    laptop_type = ntohl(net_laptop_type);
}

bool LaptopOrder::IsValid() {
    return (customer_id != -1);
}

void LaptopOrder::Print() {
    std::cout << "id " << customer_id << " ";
    std::cout << "num " << order_number << " ";
    std::cout << "type " << laptop_type << std::endl;
}

LaptopInfo::LaptopInfo() {
    customer_id = -1;
    order_number = -1;
    laptop_type = -1;
    engineer_id = -1;
    admin_id = -1;
}

void LaptopInfo::SetInfo(int id, int number, int type, int engid, int expid) {
    customer_id = id;
    order_number = number;
    laptop_type = type;
    engineer_id = engid;
    admin_id = expid;
}

void LaptopInfo::CopyOrder(LaptopOrder order) {
    customer_id = order.GetCustomerId();
    order_number = order.GetOrderNumber();
    laptop_type = order.GetLaptopType();
}

void LaptopInfo::SetEngineerId(int id) { engineer_id = id; }

void LaptopInfo::SetAdminId(int id) { admin_id = id; }

int LaptopInfo::GetCustomerId() { return customer_id; }

int LaptopInfo::GetOrderNumber() { return order_number; }

int LaptopInfo::GetLaptopType() { return laptop_type; }

int LaptopInfo::GetEngineerId() { return engineer_id; }

int LaptopInfo::GetAdminId() { return admin_id; }

int LaptopInfo::Size() {
    return sizeof(customer_id) + sizeof(order_number) + sizeof(laptop_type)
           + sizeof(engineer_id) + sizeof(admin_id);
}

void LaptopInfo::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_order_number = htonl(order_number);
    int net_laptop_type = htonl(laptop_type);
    int net_engineer_id = htonl(engineer_id);
    int net_admin_id = htonl(admin_id);
    int offset = 0;

    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(buffer + offset, &net_laptop_type, sizeof(net_laptop_type));
    offset += sizeof(net_laptop_type);
    memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
    offset += sizeof(net_engineer_id);
    memcpy(buffer + offset, &net_admin_id, sizeof(net_admin_id));

}

void LaptopInfo::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_order_number;
    int net_laptop_type;
    int net_engineer_id;
    int net_admin_id;
    int offset = 0;

    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(&net_laptop_type, buffer + offset, sizeof(net_laptop_type));
    offset += sizeof(net_laptop_type);
    memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
    offset += sizeof(net_engineer_id);
    memcpy(&net_admin_id, buffer + offset, sizeof(net_admin_id));

    customer_id = ntohl(net_customer_id);
    order_number = ntohl(net_order_number);
    laptop_type = ntohl(net_laptop_type);
    engineer_id = ntohl(net_engineer_id);
    admin_id = ntohl(net_admin_id);
}

bool LaptopInfo::IsValid() {
    return (customer_id != -1);
}

void LaptopInfo::Print() {
    std::cout << "id " << customer_id << " ";
    std::cout << "num " << order_number << " ";
    std::cout << "type " << laptop_type << " ";
    std::cout << "engid " << engineer_id << " ";
    std::cout << "expid " << admin_id << std::endl;
}

CustomerRecord::CustomerRecord(int customerId, int lastOrder) : customer_id(customerId), last_order(lastOrder) {}

int CustomerRecord::getCustomerId() const {
    return customer_id;
}

int CustomerRecord::getLastOrder() const {
    return last_order;
}

void CustomerRecord::Unmarshal(char *buffer) {
    int net_customer_id = -1;
    int net_last_order = -1;
    int offset = 0;

    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_last_order, buffer + offset, sizeof(net_last_order));

    customer_id = ntohl(net_customer_id);
    last_order = ntohl(net_last_order);
}

void CustomerRecord::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_last_order_number = htonl(last_order);
    int offset = 0;
    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_last_order_number, sizeof(net_last_order_number));
}

void ReplicationRequest::Marshal(char *buffer) {
    vid = 1;
    int net_vid = htonl(vid);
    int net_factory_id = htonl(factory_id);
    int net_committed_index = htonl(committed_index);
    int net_last_index = htonl(last_index);
    int net_opcode = htonl(operation.opcode);
    int net_arg1 = htonl(operation.arg1);
    int net_arg2 = htonl(operation.arg2);
    int offset = 0;

    memcpy(buffer + offset, &net_vid, sizeof(net_factory_id));
    offset += sizeof(net_vid);
    memcpy(buffer + offset, &net_factory_id, sizeof(net_factory_id));
    offset += sizeof(net_factory_id);
    memcpy(buffer + offset, &net_committed_index, sizeof(net_committed_index));
    offset += sizeof(net_committed_index);
    memcpy(buffer + offset, &net_last_index, sizeof(net_last_index));
    offset += sizeof(net_last_index);
    memcpy(buffer + offset, &net_opcode, sizeof(net_opcode));
    offset += sizeof(net_opcode);
    memcpy(buffer + offset, &net_arg1, sizeof(net_opcode));
    offset += sizeof(net_arg1);
    memcpy(buffer + offset, &net_arg2, sizeof(net_arg1));

    ReplicationRequest request;
    request.Unmarshal(buffer);
    std::cout << request << std::endl;
}

int ReplicationRequest::Unmarshal(char *buffer) {
    int net_vid;
    int net_factory_id;
    int net_committed_index;
    int net_last_index;
    int net_opcode;
    int net_arg1;
    int net_arg2;
    int offset = 0;

    memcpy(&net_vid, buffer + offset, sizeof(net_vid));
    offset += sizeof(net_vid);
    memcpy(&net_factory_id, buffer + offset, sizeof(net_factory_id));
    offset += sizeof(net_factory_id);
    memcpy(&net_committed_index, buffer + offset, sizeof(net_committed_index));
    offset += sizeof(net_committed_index);
    memcpy(&net_last_index, buffer + offset, sizeof(net_last_index));
    offset += sizeof(net_last_index);
    memcpy(&net_opcode, buffer + offset, sizeof(net_opcode));
    offset += sizeof(net_opcode);
    memcpy(&net_arg1, buffer + offset, sizeof(net_arg1));
    offset += sizeof(net_arg1);
    memcpy(&net_arg2, buffer + offset, sizeof(net_arg2));

    vid = htonl(net_vid);
    factory_id = ntohl(net_factory_id);
    committed_index = ntohl(net_committed_index);
    last_index = ntohl(net_last_index);
    operation.opcode = ntohl(net_opcode);
    operation.arg1 = ntohl(net_arg1);
    operation.arg2 = ntohl(net_arg2);
    return 2;
}

std::ostream &operator<<(std::ostream &os, const ReplicationRequest &request) {
    os << " factory_id: " << request.factory_id << " committed_index: "
       << request.committed_index << " last_index: " << request.last_index << " operation: " << request.operation.opcode
       << "\t" << request.operation.arg1 << "\t" << request.operation.arg2;
    return os;
}

