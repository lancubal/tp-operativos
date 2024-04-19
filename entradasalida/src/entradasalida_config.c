//
// Created by utnso on 4/6/24.
//

#include "entradasalida_config.h"

/**
 * @brief Carga la configuración de Entrada/Salida desde un archivo.
 *
 * Esta función se encarga de cargar la configuración de Entrada/Salida desde un archivo de configuración.
 * La configuración incluye la dirección IP y el puerto del Kernel, y la dirección IP y el puerto de la Memoria.
 *
 * @param path La ruta al archivo de configuración.
 * @return Un puntero a una estructura entradasalida_config_t que contiene la configuración de Entrada/Salida.
 */
entradasalida_config_t* entradasalidaConfigLoad(char* path) {
    // Obtenemos la configuración desde el archivo
    t_config* config = getConfig(path);
    // Creamos una nueva estructura para almacenar la configuración de Entrada/Salida
    entradasalida_config_t* entradasalidaConfig = malloc(sizeof(entradasalida_config_t));

    // Cargamos la dirección IP del Kernel
    entradasalidaConfig->ipKernel = config_get_string_value(config, "IP_KERNEL");
    // Cargamos el puerto del Kernel
    entradasalidaConfig->puertoKernel = config_get_string_value(config, "PUERTO_KERNEL");
    // Cargamos la dirección IP de la Memoria
    entradasalidaConfig->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    // Cargamos el puerto de la Memoria
    entradasalidaConfig->puertoMemoria = config_get_string_value(config, "PUERTO_MEMORIA");

    // Registramos un mensaje indicando que la configuración ha sido cargada
    log_info(logger, "Configuracion cargada");

    // Retornamos la configuración de Entrada/Salida
    return entradasalidaConfig;
}