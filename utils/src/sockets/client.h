//
// Created by utnso on 4/5/24.
//

#ifndef TP_2024_1C_GRUPO_CLIENT_H
#define TP_2024_1C_GRUPO_CLIENT_H

#include<unistd.h>
#include<netdb.h>
#include <pthread.h>
#include <errno.h>
#include <utils/logger.h>

typedef struct {
    int memoriaSocket;
    int kernelSocket;
    int socketCPU;
    int interruptSocket;
    int dispatchSocket;
} socketsT;

int connectToServer(char*, char*);
void disconnectClient(int);
extern socketsT* sockets;

#endif //TP_2024_1C_GRUPO_CLIENT_H
