#include <stdlib.h>
#include <stdio.h>
#include <utils/logger.h>
#include <sockets/server.h>
#include <utils/config.h>
#include "sockets/client.h"

int main(int argc, char* argv[]) {
    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando I/O");

    //Obtener datos de configuracion
    t_config* config = getConfig(argv[1]);
    char* ipKernel = config_get_string_value(config, "IP_KERNEL");
    char* puertoKernel = config_get_string_value(config, "PUERTO_KERNEL");
    char* ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    char* puertoMemoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "Configuracion cargada");

    //Conectar a Kernel
    int socketKernel = connectToServer(ipKernel, puertoKernel);
    if(socketKernel == -1) {
        log_error(logger, "No se pudo conectar al Kernel");
        return -1;
    }
    log_info(logger, "Conectado al Kernel en: %s:%s", ipKernel, puertoKernel);

    //Conectar a memoria
    int socketMemoria = connectToServer(ipMemoria, puertoMemoria);
    if(socketMemoria == -1) {
        log_error(logger, "No se pudo conectar a la Memoria");
        return -1;
    }
    log_info(logger, "Conectado a memoria en: %s:%s", ipMemoria, puertoMemoria);

    //Finalizar
    log_destroy(logger);
    config_destroy(config);
    return 0;
}
