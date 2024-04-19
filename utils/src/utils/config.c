//
// Created by utnso on 4/5/24.
//

#include "config.h"

/**
 * @brief Obtiene la configuración desde un archivo.
 *
 * Esta función carga la configuración desde un archivo ubicado en la ruta especificada.
 * Si no se puede leer el archivo de configuración, la función imprime un mensaje de error y termina la ejecución.
 *
 * @param path La ruta al archivo de configuración.
 * @return Un puntero a la configuración cargada.
 */
t_config* getConfig(char* path)
{
    // Registra un mensaje de información indicando que se está cargando la configuración
    log_info(logger, "Cargando configuración en: %s\n", path);

    // Crea la configuración a partir del archivo en la ruta especificada
    t_config* nuevo_config = config_create(path);

    // Si no se pudo crear la configuración (es decir, si no se pudo leer el archivo de configuración)
    if (nuevo_config == NULL) {
        // Imprime un mensaje de error
        printf("No se pudo leer el archivo de configuración.\n");

        // Termina la ejecución
        exit(1);
    }

    // Registra un mensaje de información indicando que la configuración se cargó exitosamente
    log_info(logger, "Configuración cargada exitosamente.\n");

    // Devuelve la configuración cargada
    return nuevo_config;
}
