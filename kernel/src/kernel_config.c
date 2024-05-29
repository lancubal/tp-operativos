//
// Created by utnso on 4/6/24.
//

#include "kernel_config.h"

/**
 * @brief Carga la configuración del Kernel desde un archivo.
 *
 * Esta función se encarga de cargar la configuración del Kernel desde un archivo de configuración.
 * La configuración incluye la dirección IP y el puerto del Kernel, la dirección IP y el puerto de la Memoria,
 * la dirección IP y el puerto de la CPU, el algoritmo de planificación, los recursos, las instancias de recursos
 * y el grado de multiprogramación.
 *
 * @param path La ruta al archivo de configuración.
 * @return Un puntero a una estructura kernel_config_t que contiene la configuración del Kernel.
 */
void kernelConfigLoad(char *path) {
    // Obtenemos la configuración desde el archivo
    t_config *config = getConfig(path);
    // Creamos una nueva estructura para almacenar la configuración del Kernel
    kernel_config = malloc(sizeof(kernel_config_t));

    // Cargamos la dirección IP del Kernel
    kernel_config->ipKernel = config_get_string_value(config, "IP_KERNEL");
    // Cargamos el puerto de escucha del Kernel
    kernel_config->puertoEscucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    // Cargamos la dirección IP de la Memoria
    kernel_config->ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    // Cargamos el puerto de la Memoria
    kernel_config->puertoMemoria = config_get_string_value(config, "PUERTO_MEMORIA");
    // Cargamos la dirección IP de la CPU
    kernel_config->ipCPU = config_get_string_value(config, "IP_CPU");
    // Cargamos el puerto de despacho de la CPU
    kernel_config->puertoCPUDispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    // Cargamos el puerto de interrupción de la CPU
    kernel_config->puertoCPUInterrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    // Cargamos el algoritmo de planificación
    kernel_config->algoritmoPlanificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    // Cargamos los recursos
    kernel_config->recursos = config_get_string_value(config, "RECURSOS");
    // Cargamos las instancias de recursos
    kernel_config->instanciasRecursos = config_get_string_value(config, "INSTANCIAS_RECURSOS");
    // Cargamos el grado de multiprogramación
    kernel_config->gradoMultiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");

    // Registramos un mensaje indicando que la configuración ha sido cargada
    log_info(logger, "Configuracion cargada");
}

