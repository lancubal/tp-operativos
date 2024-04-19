
#include <utils/config.h>
#include <kernel_config.h>
#include "kernel_user_interface.h"
#include <kernel_conexion.h>
#include <signal.h>


void sighandler(int s);

t_log *logger;
socketsT sockets;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);

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


    //Hacer un Thread por cada inicio de Server y conexion en un funcion USER INICIAR CONEXIONES
    iniciarConexiones(kernelConfig,&sockets);


    pthread_t kernel_thread;
    pthread_create(&kernel_thread,NULL,(void *)phread_server_escuchar,&sockets.kernelSocket);

    kernelUserInterfaceStart(&sockets);

    // int fin; // Mas adelante cambiarlo por socket de Kernel
    // pthread_join(kernel_thread,(void*) &fin);  // Deberia recibir señal para cerrar el thread

    //Finalizar
    fin_conexion(logger,&sockets);
    close(kernel_thread);
    return 0;
}


void sighandler(int s) {
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    fin_conexion(logger,&sockets);
    exit(0);
}