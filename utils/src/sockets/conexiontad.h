//
// Created by utnso on 4/8/24.
//

#ifndef TP_2024_1C_GRUPO_CONEXIONTAD_H
#define TP_2024_1C_GRUPO_CONEXIONTAD_H

#include <stdlib.h>

typedef struct {
    char* ip;
    char* puerto;
    char* proceso;
} conexionArgsT;

conexionArgsT * createConexionArgs(char* ip, char* puerto, char* proceso);
void destroyConexionArgs(conexionArgsT* args);


#endif //TP_2024_1C_GRUPO_CONEXIONTAD_H
