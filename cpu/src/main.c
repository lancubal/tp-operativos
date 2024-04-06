#include <stdlib.h>
#include <stdio.h>
#include <cpu_config.h>
#include <cpu_server.h>
#include <cpu_client.h>

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
    iniciarDispatch(cpuConfig->ipCPU,cpuConfig->puertoEscuchaDispatch);
    iniciarInterrupt(cpuConfig->ipCPU,cpuConfig->puertoEscuchaInterrupt);

    //Conectar a memoria
    conectarAMemoria(cpuConfig->ipMemoria,cpuConfig->puertoMemoria);

    //Finalizar
    log_destroy(logger);
    return 0;
}

