//
// Created by utnso on 4/5/24.
//

#include "config.h"

t_config* getConfig(char* path)
{
    log_info(logger, "Cargando configuración en: %s\n", path);
    t_config* nuevo_config = config_create(path);
    if (nuevo_config == NULL) {
        printf("No se pudo leer el archivo de configuración.\n");
        exit(1);
    }
    log_info(logger, "Configuración cargada exitosamente.\n");
    return nuevo_config;
}
