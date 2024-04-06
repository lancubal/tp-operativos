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
    log_info(logger, "Iniciando kernel");

    //Obtener datos de configuracion
    t_config* config = getConfig(argv[1]);
    char* ipKernel = config_get_string_value(config, "IP_KERNEL");
    char* puertoEscucha = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA"));
    char* ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    char* puertoMemoria = string_from_format("%d", config_get_int_value(config, "PUERTO_MEMORIA"));
    char* ipCPU = config_get_string_value(config, "IP_CPU");
    char* puertoCPUDispatch = string_from_format("%d", config_get_int_value(config, "PUERTO_CPU_DISPATCH"));
    char* puertoCPUInterrupt = string_from_format("%d", config_get_int_value(config, "PUERTO_CPU_INTERRUPT"));
    char* algoritmoPlanificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    int quantum = config_get_int_value(config, "QUANTUM");
    char* recursos = config_get_string_value(config, "RECURSOS");
    char* instanciasRecursos = config_get_string_value(config, "INSTANCIAS_RECURSOS");
    int gradoMultiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    log_info(logger, "Configuracion cargada");

    //Iniciar servidor de Kernel
    int kernelSocket = startServer(ipKernel, puertoEscucha);
    if(kernelSocket == -1) {
        log_error(logger, "No se pudo iniciar servidor de Kernel");
        return -1;
    }
    log_info(logger, "Servidor de Kernel iniciado en: %s:%s", ipKernel, puertoEscucha);

    //Conectar a Memoria
    int socketMemoria = connectToServer(ipMemoria, puertoMemoria);
    if(socketMemoria == -1) {
        log_error(logger, "No se pudo conectar a Memoria");
        return -1;
    }
    log_info(logger, "Conectado a memoria en: %s:%s", ipMemoria, puertoMemoria);

    //Conectar a CPU Dispatch
    int socketCPUDispatch = connectToServer(ipCPU, puertoCPUDispatch);
    if(socketCPUDispatch == -1) {
        log_error(logger, "No se pudo conectar a CPU Dispatch");
        return -1;
    }
    log_info(logger, "Conectado a CPU Dispatch en: %s:%s", ipCPU, puertoCPUDispatch);

    //Conectar a CPU Interrupt
    int socketCPUInterrupt = connectToServer(ipCPU, puertoCPUInterrupt);
    if(socketCPUInterrupt == -1) {
        log_error(logger, "No se pudo conectar a CPU Interrupt");
        return -1;
    }
    log_info(logger, "Conectado a CPU Interrupt en: %s:%s", ipCPU, puertoCPUInterrupt);

    //Consola interactiva
    while(true) {
        printf("1. Ejecutar Script de Operaciones\n");
        printf("2. Iniciar proceso\n");
        printf("3. Finalizar proceso\n");
        printf("4. Iniciar planificación\n");
        printf("5. Detener planificación\n");
        printf("6. Listar procesos por estado\n");
        char* comando = readline("> ");
        if(string_is_empty(comando)) {
            break;
        }
    }

    //Finalizar
    config_destroy(config);
    log_destroy(logger);
    return 0;
}
