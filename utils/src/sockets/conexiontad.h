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
    int socketClient;
} ConectarArgsT;

ConectarArgsT* createConectarArgs(char* ip, char* puerto, char* proceso, int socketClient);

void destroyClientArgs(ConectarArgsT* args);


#endif //TP_2024_1C_GRUPO_CONEXIONTAD_H
