//
// Created by utnso on 4/6/24.
//

#include "cpu_config.h"

/**
 * @brief Carga la configuración de la CPU desde un archivo.
 *
 * Esta función se encarga de cargar la configuración de la CPU desde un archivo de configuración.
 * La configuración incluye la dirección IP y el puerto de la CPU, la dirección IP y el puerto de la memoria,
 * la cantidad de entradas de la TLB y el algoritmo de la TLB.
 *
 * @param path La ruta al archivo de configuración.
 * @return Un puntero a una estructura cpu_config_t que contiene la configuración de la CPU.
 */
void cpuConfigLoad(char* path) {
    // Obtenemos la configuración desde el archivo
    t_config* config = getConfig(path);
    // Creamos una nueva estructura para almacenar la configuración de la CPU
    cpu_config = malloc(sizeof(cpu_config_t));

    // Cargamos la dirección IP de la CPU
    cpu_config->ipCPU = config_get_string_value(config, "IP_CPU");
    log_info(logger, "IP CPU: %s", cpu_config->ipCPU);
    // Cargamos la dirección IP de la memoria
    cpu_config->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    // Cargamos el puerto de la memoria
    cpu_config->puertoMemoria = string_from_format("%d", config_get_int_value(config, "PUERTO_MEMORIA"));
    // Cargamos el puerto de escucha del despachador
    cpu_config->puertoEscuchaDispatch = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH"));
    // Cargamos el puerto de escucha de las interrupciones
    cpu_config->puertoEscuchaInterrupt = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT"));
    // Cargamos la cantidad de entradas de la TLB
    cpu_config->cantidadEntradasTLB = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    // Cargamos el algoritmo de la TLB
    cpu_config->algoritmoTLB = config_get_string_value(config, "ALGORITMO_TLB");

    // Registramos un mensaje indicando que la configuración ha sido cargada
    log_info(logger, "Configuracion cargada");
}