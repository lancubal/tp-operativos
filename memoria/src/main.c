#include <stdlib.h>
#include <stdio.h>
#include <utils/config.h>
#include <sockets/server.h>
#include <memoria_config.h>

t_log *logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando memoria");

    //Verificar que haya un archivo de configuración
    printf("argc: %d\n", argc);
    if (argc == 1) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    memoria_config_t* memoriaConfig = memoriaConfigLoad(argv[1]);

    //Iniciar servidor de Memoria server
    int socketMemoria = iniciarServerProceso(memoriaConfig->ipMemoria, memoriaConfig->puertoEscucha, "Memoria");

    //Se crean los tad para recibir al CPU
    pthread_t cpuClientTH;
    //Se crea el hilo para recibir al CPU
    pthread_create(&cpuClientTH, NULL, (void*) waitClient, (void*) socketMemoria);

    //Se crea el tad para recibir al Kernel
    pthread_t kernelClientTH;
    //Se crea el hilo para recibir al Kernel
    pthread_create(&kernelClientTH, NULL, (void*) waitClient, (void*) socketMemoria);

    //Se inicia el hilo para recibir al CPU
    int socketCPU;
    int socketKernel;
    log_info(logger, "Esperando al cliente: CPU");
    log_info(logger, "Esperando al cliente: Kernel");
    pthread_join(cpuClientTH, (void*) &socketCPU);
    pthread_join(kernelClientTH, (void*) &socketKernel);

    //Finalizar
    //disconnectServer(socketCPU);
    log_destroy(logger);
    return 0;
}
