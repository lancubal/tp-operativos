//
// Created by utnso on 4/6/24.
//

#ifndef TP_2024_1C_GRUPO_KERNEL_CONFIG_H
#define TP_2024_1C_GRUPO_KERNEL_CONFIG_H

#include "utils/config.h"
#include "utils/logger.h"

typedef struct {
    char* ipKernel;
    char* puertoEscucha;
    char* ipMemoria;
    char* puertoMemoria;
    char* ipCPU;
    char* puertoCPUDispatch;
    char* puertoCPUInterrupt;
    char* algoritmoPlanificacion;
    int quantum;
    char* recursos;
    char* instanciasRecursos;
    int gradoMultiprogramacion;
} kernel_config_t;

kernel_config_t* kernelConfigLoad(char* path);

#endif //TP_2024_1C_GRUPO_KERNEL_CONFIG_H
