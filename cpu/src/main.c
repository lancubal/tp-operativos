#include <stdlib.h>
#include <stdio.h>
#include <cpu_config.h>
#include <sockets/client.h>
#include <utils/logger.h>
#include <commons/log.h>

t_log* logger;

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
    cpu_config_t* cpuConfig = cpuConfigLoad(argv[1]);


    //Iniciar Dispatch e Interrupt
    iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaDispatch, "Dispatch");
    iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaInterrupt, "Interrupt");

    //Conectar a memoria
    conectarA(cpuConfig->ipMemoria, cpuConfig->puertoMemoria, "Memoria");

    //Finalizar
    log_destroy(logger);
    return 0;
}