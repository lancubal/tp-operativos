#include <utils/logger.h>
#include <utils/config.h>
#include "sockets/client.h"
#include <entradasalida_config.h>

t_log* logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando I/O");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    entradasalida_config_t * entradasalidaConfig = entradasalidaConfigLoad(argv[1]);


    //Se crea el tad para el hilo de conexion a kernel
    pthread_t conectarAKernel;
    //Se asignan los argumentos para el hilo de kernel
    conexionArgsT* kernelServerArgs = createConexionArgs(entradasalidaConfig->ipKernel, entradasalidaConfig->puertoKernel, "Kernel");
    //Se crea el hilo de kernel
    pthread_create(&conectarAKernel, NULL, (void*) conectarA, (void*) kernelServerArgs);

    //Se crea el tad para el hilo de conexion a Memoria
    pthread_t conectarAMemoria;
    //Se asignan los argumentos para el hilo de Memoria
    conexionArgsT* memoriaServerArgs = createConexionArgs(entradasalidaConfig->ipMemoria, entradasalidaConfig->puertoMemoria, "Memoria");
    //Se crea el hilo de Memoria
    pthread_create(&conectarAMemoria, NULL, (void*) conectarA, (void*) memoriaServerArgs);

    //Se inician los hilos de las conexiones
    pthread_join(conectarAKernel, NULL);
    pthread_join(conectarAMemoria, NULL);

    //Finalizar
    log_destroy(logger);
    return 0;
}
