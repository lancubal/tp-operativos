//
// Created by utnso on 4/6/24.
//

#ifndef TP_2024_1C_GRUPO_CPU_CONFIG_H
#define TP_2024_1C_GRUPO_CPU_CONFIG_H

#include "utils/config.h"
#include "utils/logger.h"
#include <commons/log.h>

typedef struct {
    char* ipCPU;
    char* ipMemoria;
    char* puertoMemoria;
    char* puertoEscuchaDispatch;
    char* puertoEscuchaInterrupt;
    int cantidadEntradasTLB;
    char* algoritmoTLB;
} cpu_config_t;

extern cpu_config_t* cpu_config;

void cpuConfigLoad(char* path);

#endif //TP_2024_1C_GRUPO_CPU_CONFIG_H
