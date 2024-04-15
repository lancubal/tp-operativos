//
// Created by utnso on 4/8/24.
//

#include "conexiontad.h"

void destroyClientArgs(ConectarArgsT* args) {
    free(args);
}

ConectarArgsT * createConectarArgs(char* ip, char* puerto, char* proceso) {
    ConectarArgsT* args = malloc(sizeof(ConectarArgsT));
    args->serverIP = ip;
    args->puerto = puerto;
    args->proceso = proceso;
    return args;
}

socketsT * createSocketsT(int memoriaSocket, int kernelSocket, int interruptSocket, int dispatchSocket) {
    socketsT* sockets = malloc(sizeof(socketsT));
    sockets->memoriaSocket = memoriaSocket;
    sockets->kernelSocket = kernelSocket;
    sockets->interruptSocket = interruptSocket;
    sockets->dispatchSocket = dispatchSocket;
    return sockets;
}