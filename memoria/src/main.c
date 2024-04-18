#include <stdio.h>
#include <utils/config.h>
#include <memoria_config.h>
#include <memoria_conexion.h>
#include "sockets/networking.h"
#include <signal.h>

void sighandler(int s) {
    log_info(logger,"Terminado el Servidor Memoria");
    log_destroy(logger);
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    exit(0);
}

t_log *logger;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);
    socketsT sockets;

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

    //Iniciar conexiones
    //socketsT* sockets = iniciarConexiones(memoriaConfig);
    sockets.memoriaSocket = iniciarServerProceso(memoriaConfig->ipMemoria, memoriaConfig->puertoEscucha, "Memoria");
    while (server_escuchar(logger, "Servidor Memoria",sockets.memoriaSocket ));



    // hilo para getMessage
    // pthread_t getMessageTH;
    // pthread_create(&getMessageTH, NULL, (void*) getMessage, (void*) &sockets->socketCPU);
    // char *mensaje = malloc(sizeof (char) * 100);
    // pthread_join(getMessageTH, (void*) mensaje);
    // log_info(logger, "Mensaje recibido de CPU: %s", mensaje);
    // sendMessage("Hola CPU", sockets->socketCPU);
    //
    


    //Finalizar
    //close(sockets->memoriaSocket);
    disconnectServer(sockets.memoriaSocket);
    log_destroy(logger);
    return 0;
}
