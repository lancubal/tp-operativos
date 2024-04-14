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



