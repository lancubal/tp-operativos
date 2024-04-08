
#include <utils/config.h>
#include <sockets/server.h>
#include <sockets/client.h>
#include <kernel_config.h>
#include "kernel_user_interface.h"

t_log *logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando kernel");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    kernel_config_t * kernelConfig = kernelConfigLoad(argv[1]);

    //Iniciar servidor de Kernel
    //Se crea el tad para el hilo de Memoria server
    pthread_t kernelServer;
    //Se asignan los argumentos para el hilo de Memoria server
    conexionArgsT* kernelServerArgs = createConexionArgs(kernelConfig->ipKernel, kernelConfig->puertoEscucha, "Kernel");
    //Se crea el hilo de Memoria server
    pthread_create(&kernelServer, NULL, (void*) iniciarServerProceso, (void*) kernelServerArgs);
    //se inicia el hilo del servidor de Memoria y se devuelve el socket del cliente
    int socketCliente;


    //Se crea el tad para el hilo de conexion a Memoria
    pthread_t conectarAMemoria;
    //Se asignan los argumentos para el hilo de Memoria
    conexionArgsT* memoriaServerArgs = createConexionArgs(kernelConfig->ipMemoria, kernelConfig->puertoMemoria, "Memoria");
    //Se crea el hilo de Memoria
    pthread_create(&conectarAMemoria, NULL, (void*) conectarA, (void*) memoriaServerArgs);

    //Se crea el tad para el hilo de conexion a CPU Dispatch
    pthread_t conectarACPUDispatch;
    //Se asignan los argumentos para el hilo de CPU Dispatch
    conexionArgsT* cpuDispatchServerArgs = createConexionArgs(kernelConfig->ipCPU, kernelConfig->puertoCPUDispatch, "CPU Dispatch");
    //Se crea el hilo de CPU Dispatch
    pthread_create(&conectarACPUDispatch, NULL, (void*) conectarA, (void*) cpuDispatchServerArgs);

    //Se crea el tad para el hilo de conexion a CPU Interrupt
    pthread_t conectarACPUInterrupt;
    //Se asignan los argumentos para el hilo de CPU Interrupt
    conexionArgsT* cpuInterruptServerArgs = createConexionArgs(kernelConfig->ipCPU, kernelConfig->puertoCPUInterrupt, "CPU Interrupt");
    //Se crea el hilo de CPU Interrupt
    pthread_create(&conectarACPUInterrupt, NULL, (void*) conectarA, (void*) cpuInterruptServerArgs);

    //Consola interactiva
    kernelUserInterfaceStart();

    //Se inician los hilos de las conexiones
    int socketKernelCliente;
    pthread_join(kernelServer, (void*) &socketKernelCliente);
    int socketMemoriaServer;
    pthread_join(conectarAMemoria, (void*) &socketMemoriaServer);
    int socketCPUDispatchServer;
    pthread_join(conectarACPUDispatch, (void*) &socketCPUDispatchServer);
    int socketCPUInterruptServer;
    pthread_join(conectarACPUInterrupt, (void*) &socketCPUInterruptServer);

    //Finalizar
    log_destroy(logger);
    return 0;
}
