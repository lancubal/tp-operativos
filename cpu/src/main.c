#include <cpu_config.h>
#include <sockets/client.h>
#include <sockets/server.h>
#include <commons/log.h>
#include <pthread.h>

t_log* logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando CPU");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    cpu_config_t* cpuConfig = cpuConfigLoad(argv[1]);

    //Se crea el tad para el hilo de Dispatch
    pthread_t dispatchServer;
    //Se asignan los argumentos para el hilo de Dispatch
    conexionArgsT* dispatchServerArgs = createConexionArgs(cpuConfig->ipCPU, cpuConfig->puertoEscuchaDispatch, "Dispatch");
    //Se crea el hilo de Dispatch
    pthread_create(&dispatchServer, NULL, (void*) iniciarServerProceso, (void*) dispatchServerArgs);

    //Se crea el tad para el hilo de Interrupt
    pthread_t interruptServer;
    //Se asignan los argumentos para el hilo de Interrupt
    conexionArgsT* interruptServerArgs = createConexionArgs(cpuConfig->ipCPU, cpuConfig->puertoEscuchaInterrupt, "Interrupt");
    //Se crea el hilo de Interrupt
    pthread_create(&interruptServer, NULL, (void*) iniciarServerProceso, (void*) interruptServerArgs);

    //Se crea el tad para el hilo de conexion a Memoria
    pthread_t conectarAMemoria;
    //Se asignan los argumentos para el hilo de Memoria
    conexionArgsT* memoriaServerArgs = createConexionArgs(cpuConfig->ipMemoria, cpuConfig->puertoMemoria, "Memoria");

    //Se crea el hilo de Memoria
    pthread_create(&conectarAMemoria, NULL, (void*) conectarA, (void*) memoriaServerArgs);

    //Se inician los hilos de las conexiones
    int socketInterruptCliente;
    pthread_join(interruptServer, (void*) &socketInterruptCliente);
    int socketDispatchCliente;
    pthread_join(dispatchServer, (void*) &socketDispatchCliente);
    pthread_join(conectarAMemoria, NULL);

    //Finalizar
    disconnectServer(socketInterruptCliente);
    disconnectServer(socketDispatchCliente);
    log_destroy(logger);
    return 0;
}