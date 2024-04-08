//
// Created by utnso on 4/8/24.
//

#include "conexiontad.h"

void destroyConexionArgs(conexionArgsT * args) {
    free(args);
}

conexionArgsT * createConexionArgs(char* ip, char* puerto, char* proceso) {
    conexionArgsT* args = malloc(sizeof(conexionArgsT));
    args->ip = ip;
    args->puerto = puerto;
    args->proceso = proceso;
    return args;
}

