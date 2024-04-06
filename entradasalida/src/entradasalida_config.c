//
// Created by utnso on 4/6/24.
//

#include "entradasalida_config.h"

entradasalida_config_t* entradasalidaConfigLoad(char* path) {
    t_config* config = getConfig(path);
    entradasalida_config_t* entradasalidaConfig = malloc(sizeof(entradasalida_config_t));

    entradasalidaConfig->ipKernel = config_get_string_value(config, "IP_KERNEL");
    entradasalidaConfig->puertoKernel = config_get_string_value(config, "PUERTO_KERNEL");
    entradasalidaConfig->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    entradasalidaConfig->puertoMemoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "Configuracion cargada");

    return entradasalidaConfig;
}