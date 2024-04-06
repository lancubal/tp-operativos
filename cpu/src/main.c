#include <stdlib.h>
#include <stdio.h>
#include <utils/config.h>
#include <sockets/server.h>
#include <utils/logger.h>
#include <sockets/client.h>

int main(int argc, char* argv[]) {
    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando CPU");

    //Obtener datos de configuracion
    t_config* config = getConfig(argv[1]);
    char* ipCPU = config_get_string_value(config, "IP_CPU");
    char* ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    char* puertoMemoria = string_from_format("%d", config_get_int_value(config, "PUERTO_MEMORIA"));
    char* puertoEscuchaDispatch = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH"));
    char* puertoEscuchaInterrupt = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT"));
    int cantidadEntradasTLB = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    char* algoritmoTLB = config_get_string_value(config, "ALGORITMO_TLB");
    log_info(logger, "Configuracion cargada");

    //Iniciar servidor de CPU Dispatch
    int socketDispatch = startServer(ipCPU, puertoEscuchaDispatch);
    if(socketDispatch == -1) {
        log_error(logger, "Error al iniciar servidor de CPU Dispatch");
        return -1;
    }
    log_info(logger, "Servidor de CPU Dispatch iniciado en: %s:%s", ipCPU, puertoEscuchaDispatch);

    //Iniciar servidor de CPU Interrupt
    int socketInterrupt = startServer(ipCPU, puertoEscuchaInterrupt);
    if(socketInterrupt == -1) {
        log_error(logger, "Error al iniciar servidor de CPU Interrupt");
        return -1;
    }
    log_info(logger, "Servidor de CPU Interrupt iniciado en: %s:%s", ipCPU, puertoEscuchaInterrupt);

    //Conectar a memoria
    int socketMemoria = connectToServer(ipMemoria, puertoMemoria);
    if(socketMemoria == -1) {
        log_error(logger, "Error al conectar a memoria");
        return -1;
    }
    log_info(logger, "Conectado a memoria en: %s:%s", ipMemoria, puertoMemoria);

    //Finalizar
    config_destroy(config);
    log_destroy(logger);
    return 0;
}
