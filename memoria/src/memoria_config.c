//
// Created by utnso on 4/6/24.
//

#include "memoria_config.h"

memoria_config_t* memoriaConfigLoad(char* path) {
    t_config* config = getConfig(path);
    memoria_config_t* memoriaConfig = malloc(sizeof(memoria_config_t));

    memoriaConfig->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    memoriaConfig->puertoEscucha = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA"));
    memoriaConfig->tamMemoria = config_get_int_value(config, "TAM_MEMORIA");
    memoriaConfig->tamPagina = config_get_int_value(config, "TAM_PAGINA");
    memoriaConfig->pathInstrucciones = config_get_string_value(config, "PATH_INSTRUCCIONES");
    memoriaConfig->retardoRespuesta = config_get_int_value(config, "RETARDO_RESPUESTA");
    log_info(logger, "Configuracion cargada");

    return memoriaConfig;
}