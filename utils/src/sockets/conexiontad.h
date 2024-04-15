//
// Created by utnso on 4/8/24.
//

#ifndef TP_2024_1C_GRUPO_CONEXIONTAD_H
#define TP_2024_1C_GRUPO_CONEXIONTAD_H

#include <stdlib.h>

typedef struct {
    char* serverIP;
    char* puerto;
    char* proceso;
} ConectarArgsT;

typedef struct {
    int memoriaSocket;
    int kernelSocket;
    int socketCPU;
    int interruptSocket;
    int dispatchSocket;
} socketsT;

socketsT* createSocketsT(int memoriaSocket, int kernelSocket, int interruptSocket, int dispatchSocket);
ConectarArgsT* createConectarArgs(char* ip, char* puerto, char* proceso);

void destroyClientArgs(ConectarArgsT* args);


#endif //TP_2024_1C_GRUPO_CONEXIONTAD_H
