cmake_minimum_required(VERSION 3.23)
project(PA2)

set(CMAKE_CXX_STANDARD 11)

add_executable(client Socket.h Socket.cpp Messages.cpp Messages.h ClientThread.h ClientThread.cpp ClientStub.cpp ClientStub.h ClientTimer.h ClientTimer.cpp ClientSocket.h ClientSocket.cpp ClientMain.cpp)
add_executable(server Socket.h Socket.cpp Messages.h Messages.cpp ServerThread.h ServerThread.cpp ServerStub.cpp ServerStub.h ServerMain.cpp ServerStorage.cpp ServerStorage.h ServerSocket.cpp ServerSocket.h RWLock.h ClientStub.h ClientStub.cpp ClientSocket.cpp ClientSocket.h)

