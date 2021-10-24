//
// Created by devilox on 8/1/21.
//
//-----------------------------//
#ifndef DNETWORK_DSERVER_H
#define DNETWORK_DSERVER_H
//-----------------------------//
#include <iostream>
#include <unordered_map>
#include <coroutine>
#include <future>
#include <shared_mutex>
//-----------------------------//
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
//-----------------------------//
#include "dClientData.h"
//-----------------------------//
class dServer {
public:
    void init(int tPortTCP);
    void deinit();

    void start();
protected:
    std::unordered_map <int, dClientData*>      mClients;

    ///---TODO: change void to smth---///
    std::future <void>                          mReadSocketsThread;     //---For now only 1024 sockets---//

    std::shared_mutex                           mMapMutex;

    //----------//

    void readSocketsCallback();

    void acceptConnection();
private:
    std::atomic_bool    mRunning            = false;

    int                 mEpollFD;
    int                 mServerSocket;
    sockaddr_in         mServerStruct       = {};
    epoll_event         mServerEvent        = {};
};
//-----------------------------//
#endif //DNETWORK_DSERVER_H
