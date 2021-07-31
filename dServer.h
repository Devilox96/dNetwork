//
// Created by devilox on 8/1/21.
//
//-----------------------------//
#ifndef DNETWORK_DSERVER_H
#define DNETWORK_DSERVER_H
//-----------------------------//
#include <unordered_map>
#include <coroutine>
#include <future>
#include <shared_mutex>
//-----------------------------//
#include "dClientData.h"
//-----------------------------//
class dServer {
public:
protected:
    std::unordered_map <int, dClientData*>      mClients;

    ///---TODO: change void to smth---///
    std::future <void>                          mReadSocketsThread;     //---For now only 1024 sockets---//

    std::shared_mutex                           mMapMutex;

    //----------//

    void readSocketsCallback();
};
//-----------------------------//
#endif //DNETWORK_DSERVER_H
