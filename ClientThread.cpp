#include "ClientThread.h"
#include "Messages.h"

#include <iostream>
#include <thread>

ClientThreadClass::ClientThreadClass() {}

void ClientThreadClass::ThreadBody(std::string ip, int port, int id, int orders, int type) {
    customer_id = id;
    num_orders = orders;
    laptop_type = type;
    if (!stub.Init(ip, port)) {
        std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
        return;
    }
    stub.SendIdentification(1);
    if (laptop_type == 2) {
        LaptopOrder order;
        order.SetOrder(customer_id, -1, laptop_type);
        CustomerRecord record = stub.ReadRecord(order);
        if (!record.IsValid()) {
            std::cout << "Invalid record " << customer_id << std::endl;
            return;
        }
        std::cout << "record's last order by " << record.getLastOrder() << std::endl;
        return;
    }
    for (int i = 0; i < num_orders; i++) {
        LaptopOrder order;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        timer.Start();
        switch (laptop_type) {
            case 1: {
                order.SetOrder(customer_id, i, laptop_type);
                LaptopInfo laptop = stub.OrderLaptop(order);
                if (!laptop.IsValid()) {
                    std::cout << "Invalid laptop " << customer_id << std::endl;
                    break;
                }
                std::cout << "laptop produced by " << laptop.GetEngineerId() << " for cust: " << laptop.GetCustomerId()
                          << "of # " << laptop.GetOrderNumber() << std::endl;
                break;
            }
            case 3: {
                order.SetOrder(i, -1, laptop_type);
                CustomerRecord record = stub.ReadRecord(order);
                if (!record.IsValid()) {
                    std::cout << "Invalid record " << customer_id << std::endl;
                    return;
                }
                if (record.getCustomerId() != -1)
                    std::cout << record.getCustomerId() << "\t" << record.getLastOrder() << std::endl;
                break;
            }
            default:
                std::cout << "request invalid" << std::endl;
                break;
        }
        timer.EndAndMerge();
    }

}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}

