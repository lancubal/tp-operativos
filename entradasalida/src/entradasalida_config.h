//
// Created by utnso on 4/6/24.
//

#ifndef TP_2024_1C_GRUPO_ENTRADASALIDA_CONFIG_H
#define TP_2024_1C_GRUPO_ENTRADASALIDA_CONFIG_H

#include "utils/config.h"
#include "utils/logger.h"
#include <commons/log.h>

typedef struct {
    char* ipKernel;
    char* puertoKernel;
    char* ipMemoria;
    char* puertoMemoria;
} entradasalida_config_t;

entradasalida_config_t* entradasalidaConfigLoad(char* path);

#endif //TP_2024_1C_GRUPO_ENTRADASALIDA_CONFIG_H
