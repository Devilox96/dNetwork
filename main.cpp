#include "dServer.h"
//-----------------------------//
int main() {
    dServer Server;
    Server.init(50000);
    Server.start();

    return 0;
}
