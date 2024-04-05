//
// Created by utnso on 4/5/24.
//

#include "config.h"

t_config* getConfig(void)
{
    t_config* nuevo_config = config_create("../cliente.config");
    if (nuevo_config == NULL) {
        printf("No se pudo leer el archivo de configuración.");
        exit(1);
    }

    return nuevo_config;
}
