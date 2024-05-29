//
// Created by utnso on 4/6/24.
//

#include "memoria_config.h"

/**
 * @brief Carga la configuración de la Memoria desde un archivo.
 *
 * Esta función se encarga de cargar la configuración de la Memoria desde un archivo de configuración.
 * La configuración incluye la dirección IP de la Memoria, el puerto de escucha, el tamaño de la memoria,
 * el tamaño de la página, la ruta a las instrucciones y el retardo de respuesta.
 *
 * @param path La ruta al archivo de configuración.
 * @return Un puntero a una estructura memoria_config_t que contiene la configuración de la Memoria.
 */
void memoriaConfigLoad(char* path) {
    // Obtenemos la configuración desde el archivo
    t_config* config = getConfig(path);
    // Creamos una nueva estructura para almacenar la configuración de la Memoria
    memoria_config = malloc(sizeof(memoria_config_t));

    // Cargamos la dirección IP de la Memoria
    memoria_config->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    // Cargamos el puerto de escucha de la Memoria
    memoria_config->puertoEscucha = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA"));
    // Cargamos el tamaño de la memoria
    memoria_config->tamMemoria = config_get_int_value(config, "TAM_MEMORIA");
    // Cargamos el tamaño de la página
    memoria_config->tamPagina = config_get_int_value(config, "TAM_PAGINA");
    // Cargamos la ruta a las instrucciones
    memoria_config->pathInstrucciones = config_get_string_value(config, "PATH_INSTRUCCIONES");
    // Cargamos el retardo de respuesta
    memoria_config->retardoRespuesta = config_get_int_value(config, "RETARDO_RESPUESTA");

    // Registramos un mensaje indicando que la configuración ha sido cargada
    log_info(logger, "Configuracion cargada");
}