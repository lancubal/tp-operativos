#include <cpu_config.h>
#include <sockets/client.h>
#include <sockets/server.h>
#include <commons/log.h>
#include <pthread.h>

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

    // Se crea el tad para el hilo de Dispatch
    pthread_t dispatchServer;
    // Se asignan los argumentos para el hilo de Dispatch
    server_args_t* ip_puerto_dispatch = malloc(sizeof(server_args_t));
    ip_puerto_dispatch->ip = cpuConfig->ipCPU;
    ip_puerto_dispatch->puerto = cpuConfig->puertoEscuchaDispatch;
    ip_puerto_dispatch->proceso = "Dispatch";
    // Se crea el hilo de Dispatch
    pthread_create(&dispatchServer, NULL, (void*) iniciarServerProceso, (void*) ip_puerto_dispatch);

    // Se crea el tad para el hilo de Dispatch
    pthread_t interruptServer;
    // Se asignan los argumentos para el hilo de Interrupt
    server_args_t* ip_puerto_interrupt = malloc(sizeof(server_args_t));
    ip_puerto_interrupt->ip = cpuConfig->ipCPU;
    ip_puerto_interrupt->puerto = cpuConfig->puertoEscuchaInterrupt;
    ip_puerto_interrupt->proceso = "Interrupt";
    // Se crea el hilo de Interrupt
    pthread_create(&interruptServer, NULL, (void*) iniciarServerProceso, (void*) ip_puerto_interrupt);
    // Se espera a que los hilos terminen
    pthread_join(interruptServer, NULL);
    pthread_join(dispatchServer, NULL);

    //Conectar a memoria
    conectarA(cpuConfig->ipMemoria, cpuConfig->puertoMemoria, "Memoria");

    //Finalizar
    log_destroy(logger);
    return 0;
}