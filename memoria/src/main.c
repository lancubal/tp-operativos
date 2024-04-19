#include <stdio.h>
#include <utils/config.h>
#include <memoria_config.h>
#include <memoria_conexion.h>
#include "sockets/networking.h"
#include <signal.h>


void sighandler(int s);

t_log *logger;
socketsT sockets;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);
    

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
    iniciarConexiones(memoriaConfig,&sockets);
    
    
    while(server_escuchar(logger, "Servidor Memoria",&sockets ));



    //Finalizar
    fin_conexion(logger,&sockets);
    return 0;
}


void sighandler(int s) {
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    fin_conexion(logger,&sockets);
    exit(0);
}
