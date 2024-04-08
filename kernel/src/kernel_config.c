//
// Created by utnso on 4/6/24.
//

#include "kernel_config.h"

kernel_config_t *kernelConfigLoad(char *path) {
    t_config *config = getConfig(path);
    kernel_config_t *kernelConfig = malloc(sizeof(kernel_config_t));

    kernelConfig->ipKernel = config_get_string_value(config, "IP_KERNEL");
    kernelConfig->puertoEscucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    kernelConfig->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    kernelConfig->puertoMemoria = config_get_string_value(config, "PUERTO_MEMORIA");
    kernelConfig->ipCPU = config_get_string_value(config, "IP_CPU");
    kernelConfig->puertoCPUDispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    kernelConfig->puertoCPUInterrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    kernelConfig->algoritmoPlanificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    kernelConfig->recursos = config_get_string_value(config, "RECURSOS");
    kernelConfig->instanciasRecursos = config_get_string_value(config, "INSTANCIAS_RECURSOS");
    kernelConfig->gradoMultiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");

    log_info(logger, "Configuracion cargada");
    return kernelConfig;
}

