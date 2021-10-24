//
// Created by devilox on 8/1/21.
//
//-----------------------------//
#include "dServer.h"
//-----------------------------//
void dServer::init(int tPortTCP) {
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    int Enable = 1;

    setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &Enable, sizeof(Enable));

    mServerStruct.sin_family          = AF_INET;
    mServerStruct.sin_addr.s_addr     = INADDR_ANY;
    mServerStruct.sin_port            = htons(tPortTCP);

    //----------//

    mEpollFD = epoll_create1(0);

    if (mEpollFD < 0) {
        std::cerr << "Baaaad" << std::endl;
        return;
    }

    int EventCount;
    epoll_event Events[5];

    mServerEvent.events = EPOLLIN;
    mServerEvent.data.fd = mServerSocket;

    int Result = epoll_ctl(mEpollFD, EPOLL_CTL_ADD, mServerSocket, &mServerEvent);

    if (Result) {
        std::cerr << "Baaaaad" << std::endl;
    }
}
void dServer::deinit() {
    ///---TODO: DO NOT FORGET TO CLEAN UP ALL THE CLIENTS!!!---///
    epoll_ctl(mEpollFD, EPOLL_CTL_DEL, mServerSocket, &mServerEvent);
    close(mServerSocket);
    close(mEpollFD);
}

void dServer::start() {
    int Result = bind(mServerSocket, (struct sockaddr*)&mServerStruct, sizeof(mServerStruct));

    if (Result) {
        std::cerr << "Baaaaad" << std::endl;
        return;
    }

    listen(mServerSocket, 1);

    mRunning.store(true);

    //----------//

    epoll_event Event {};
    int EventCount;
    epoll_event Events[5];
    uint8_t ReadBuf[3];

    while (mRunning) {
        EventCount = epoll_wait(mEpollFD, Events, 5, 0);

        for (int i = 0; i < EventCount; i++) {
            int FD = Events[i].data.fd;

            if (Events[i].events & EPOLLIN) {
                if (FD == mServerSocket) {
                    std::cout << "Kek" << std::endl;
                    acceptConnection();
                } else {
                    int Count = recv(FD, ReadBuf, 3, 0);

                    std::cout << "Read" << std::endl;

                    if (Count == 0) {
                        epoll_ctl(mEpollFD, EPOLL_CTL_DEL, FD, &Events[i]);
                        close(FD);
                        std::cout << "Closing..." << std::endl;

                        continue;
                    }

                    if (Count == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            continue;
                        } else {
                            ///---TODO: handle error---///
                        }
                    }

                    Event.events = EPOLLIN | EPOLLONESHOT;
                    Event.data.fd = FD;

                    if (epoll_ctl(mEpollFD, EPOLL_CTL_MOD, FD, &Event) < 0) {
                        std::cerr << "Error" << std::endl;
                    }
                }
            } else {
                std::cout << Events[i].events << std::endl;
            }
        }
    }
}
//-----------------------------//
///---TODO: add select for reading sockets and accepting connections---///
void dServer::readSocketsCallback() {
    int EpollFD = epoll_create1(0);

    if (EpollFD == -1) {
        std::cerr << "epoll_create1 error!" << std::endl;
    }

//    epoll_ctl(EpollFD, EPOLL_CTL_DEL, 0, nullptr);

    if (close(EpollFD) == -1) {
        std::cerr << "close epoll error!" << std::endl;
    }
}

void dServer::acceptConnection() {
    int ClientSocket;
    sockaddr ClientStruct {};
    socklen_t ClientStructLen = sizeof(ClientStruct);

    ClientSocket = accept(mServerSocket, &ClientStruct, &ClientStructLen);

    ///---TODO: add proper error handling---///
    if (ClientSocket == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            fprintf(stderr, "epoll accept failed");
            std::cerr << "Error (dServer::acceptConnection): epoll accept failed!" << std::endl;
        }

        return;
    }

    epoll_event Event {};

    Event.events = EPOLLIN | EPOLLONESHOT;
    Event.data.fd = ClientSocket;

    epoll_ctl(mEpollFD, EPOLL_CTL_ADD, ClientSocket, &Event);
}