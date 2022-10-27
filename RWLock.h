//
// Created by work on 10/24/22.
//

#ifndef PA2_RWLOCK_H
#define PA2_RWLOCK_H

#include "mutex"


class ReadWriteLock {
private:
    std::mutex readMtx;
    std::mutex writeMtx;
    int readCnt;
public:
    ReadWriteLock() : readCnt(0) {}

    void readLock() {
        readMtx.lock();
        if (++readCnt == 1) {
            writeMtx.lock();
        }
        readMtx.unlock();
    }

    void readUnlock() {
        readMtx.lock();
        if (--readCnt == 0) {
            writeMtx.unlock();
        }
        readMtx.unlock();
    }

    void writeLock() {
        writeMtx.lock();
    }

    void writeUnlock() {
        writeMtx.unlock();
    }
};


#endif //PA2_RWLOCK_H
