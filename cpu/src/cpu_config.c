//
// Created by utnso on 4/6/24.
//

#include "cpu_config.h"

cpu_config_t* cpuConfigLoad(char* path) {
    t_config* config = getConfig(path);
    cpu_config_t* cpuConfig = malloc(sizeof(cpu_config_t));

    cpuConfig->ipCPU = config_get_string_value(config, "IP_CPU");
    log_info(logger, "IP CPU: %s", cpuConfig->ipCPU);
    cpuConfig->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    cpuConfig->puertoMemoria = string_from_format("%d", config_get_int_value(config, "PUERTO_MEMORIA"));
    cpuConfig->puertoEscuchaDispatch = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH"));
    cpuConfig->puertoEscuchaInterrupt = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT"));
    cpuConfig->cantidadEntradasTLB = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    cpuConfig->algoritmoTLB = config_get_string_value(config, "ALGORITMO_TLB");

    log_info(logger, "Configuracion cargada");
    return cpuConfig;
}