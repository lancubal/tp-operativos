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
    //Se crea el tad para el hilo de Memoria server
    pthread_t memoriaServer;
    //Se asignan los argumentos para el hilo de Memoria server
    conexionArgsT* memoriaServerArgs = createConexionArgs(memoriaConfig->ipMemoria, memoriaConfig->puertoEscucha, "Memoria");
    //Se crea el hilo de Memoria server
    pthread_create(&memoriaServer, NULL, (void*) iniciarServerProceso, (void*) memoriaServerArgs);
    //se inicia el hilo del servidor de Memoria y se devuelve el socket del cliente
    int socketCliente;
    pthread_join(memoriaServer, (void*) &socketCliente);

    //Finalizar
    disconnectServer(socketCliente);
    log_destroy(logger);
    return 0;
}
