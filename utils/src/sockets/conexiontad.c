//
// Created by utnso on 4/8/24.
//

#include "conexiontad.h"


void destroyClientArgs(ConectarArgsT* args) {
    free(args);
}


ConectarArgsT * createClientArgs(char* ip, char* puerto, char* proceso, int socketClient) {
    ConectarArgsT* args = malloc(sizeof(ConectarArgsT));
    args->serverIP = ip;
    args->puerto = puerto;
    args->proceso = proceso;
    args->socketClient = socketClient;
    return args;
}



