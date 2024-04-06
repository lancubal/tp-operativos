
#include <utils/config.h>
#include <sockets/server.h>
#include <sockets/client.h>
#include <kernel_config.h>
#include "kernel_user_interface.h"

t_log *logger;

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
    kernel_config_t * kernelConfig = kernelConfigLoad(argv[1]);

    //Iniciar servidor de Kernel
    iniciarServerProceso(kernelConfig->ipKernel, kernelConfig->puertoEscucha, "Kernel");

    //Conectar a Memoria
    conectarA(kernelConfig->ipMemoria, kernelConfig->puertoMemoria, "Memoria");

    //Conectar a CPU Dispatch
    conectarA(kernelConfig->ipCPU, kernelConfig->puertoCPUDispatch, "CPU Dispatch");

    //Conectar a CPU Interrupt
    conectarA(kernelConfig->ipCPU, kernelConfig->puertoCPUInterrupt, "CPU Interrupt");

    //Consola interactiva
    kernelUserInterfaceStart();

    //Finalizar
    log_destroy(logger);
    return 0;
}
