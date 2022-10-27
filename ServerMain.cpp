#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h"
#include "ServerThread.h"

int main(int argc, char *argv[]) {
    int local_port;
    int engineer_cnt = 0;

    ServerSocket socket;
    LaptopFactory factory;
    std::unique_ptr<ServerSocket> new_socket;
    std::vector<std::thread> thread_vector;

    if (argc < 4) {
        std::cout << "not enough arguments for starting, you should have at least one Peer" << std::endl;
        std::cout << argv[0] << "[port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])" << std::endl;
        return 0;
    }
    int peer_number = atoi(argv[3]);
    if ((argc - 4) / 3 != peer_number || (argc - 4) % 3 != 0) {
        std::cout << "invalid argument length to setting, please check Peer setting for start" << std::endl;
        return 0;
    }

    std::cout << "server start" << std::endl;
    // set up connection with peers
    std::vector<Peer> peers;
    for (int i = 4; i < argc; i += 3) {
        int id = atoi(argv[i]);
        string ip = argv[i + 1];
        int port = atoi(argv[i + 2]);
        peers.push_back({id, ip, port});
    }
    factory.setPeerList(peers);

    factory.Init(atoi(argv[2]));

    local_port = atoi(argv[1]);
    // only one admin
    int num_admin = 1;

    for (int i = 0; i < num_admin; i++) {
        std::thread admin_thread(&LaptopFactory::ProductionAdminThread, &factory);
        thread_vector.push_back(std::move(admin_thread));
    }

    if (!socket.Init(local_port)) {
        std::cout << "Socket initialization failed" << std::endl;
        return 0;
    }
    while ((new_socket = socket.Accept())) {
        std::thread engineer_thread(&LaptopFactory::EngineerThread, &factory, std::move(new_socket), engineer_cnt++);
        thread_vector.push_back(std::move(engineer_thread));
    }
    return 0;
}
