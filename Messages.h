#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <ostream>

class LaptopOrder {
private:
    int customer_id;
    int order_number;
    int laptop_type;

public:
    LaptopOrder();

    void operator=(const LaptopOrder &order) {
        customer_id = order.customer_id;
        order_number = order.order_number;
        laptop_type = order.laptop_type;
    }

    void SetOrder(int cid, int order_num, int type);

    int GetCustomerId();

    int GetOrderNumber();

    int GetLaptopType();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};

class LaptopInfo {
private:
    int customer_id;
    int order_number;
    int laptop_type;
    int engineer_id;
    int admin_id;

public:
    LaptopInfo();

    void operator=(const LaptopInfo &info) {
        customer_id = info.customer_id;
        order_number = info.order_number;
        laptop_type = info.laptop_type;
        engineer_id = info.engineer_id;
        admin_id = info.admin_id;
    }

    void SetInfo(int cid, int order_num, int type, int engid, int expid);

    void CopyOrder(LaptopOrder order);

    void SetEngineerId(int id);

    void SetAdminId(int id);

    int GetCustomerId();

    int GetOrderNumber();

    int GetLaptopType();

    int GetEngineerId();

    int GetAdminId();

    int Size();

    void Marshal(char *buffer);

    void Unmarshal(char *buffer);

    bool IsValid();

    void Print();
};


struct MapOp {
    int opcode;         // operation code: 1 - update value
    int arg1;           // customer_id to apply the operation
    int arg2;           // parameter for the operation
};

class CustomerRecord {
private:
    int customer_id = -1;    // copied from the read request, -1 if customer_id is not found in the map
    int last_order = -1;     // copied from the map -1 if customer_id is not found in the map

public:
    CustomerRecord() = default;

    CustomerRecord(int customerId, int lastOrder);

    int getCustomerId() const;

    int getLastOrder() const;

    void setCustomerId(int customerId) { customer_id = customerId; }

    void setLastOrder(int lastOrder) { last_order = lastOrder; }

    int Size() { return sizeof(customer_id) + sizeof(last_order); }

    void Unmarshal(char string[32]);

    bool IsValid() const { return customer_id != -1; }

    void Marshal(char string[32]);
};


class ReplicationRequest {
private:
    int vid;
    int factory_id;
    int committed_index;
    int last_index;
    MapOp operation;

public:
    ReplicationRequest() {};

    ReplicationRequest(int factoryId, int committedIndex, int lastIndex, const MapOp &operation)
            : factory_id(factoryId), committed_index(committedIndex), last_index(lastIndex), operation(operation) {};

    int getFactoryId() const {
        return factory_id;
    }

    int getCommittedIndex() const {
        return committed_index;
    }

    int getLastIndex() const {
        return last_index;
    }

    const MapOp &getOperation() const {
        return operation;
    }

    void Marshal(char *buffer);

    int Unmarshal(char *buffer);

    int Size() {
        return sizeof(vid) + sizeof(factory_id) + sizeof(committed_index) + sizeof(last_index) + sizeof(MapOp);
    };

    bool IsValid() { return vid == 1; };

    friend std::ostream &operator<<(std::ostream &os, const ReplicationRequest &request);
};

struct RequestType {
    int type = -1; // 1 stands for client, 2 stands for server
};

#endif // #ifndef __MESSAGES_H__
