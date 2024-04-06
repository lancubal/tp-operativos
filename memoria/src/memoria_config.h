//
// Created by utnso on 4/6/24.
//

#ifndef TP_2024_1C_GRUPO_MEMORIA_CONFIG_H
#define TP_2024_1C_GRUPO_MEMORIA_CONFIG_H

#include "utils/config.h"
#include "utils/logger.h"

typedef struct {
    char* ipMemoria;
    char* puertoEscucha;
    int tamMemoria;
    int tamPagina;
    char* pathInstrucciones;
    int retardoRespuesta;
} memoria_config_t;

memoria_config_t* memoriaConfigLoad(char* path);

#endif //TP_2024_1C_GRUPO_MEMORIA_CONFIG_H
