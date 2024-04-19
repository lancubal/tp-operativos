#include <utils/logger.h>
#include <utils/config.h>
#include "sockets/client.h"
#include "sockets/protocol.h"
#include <entradasalida_config.h>
#include <entradasalida_conexion.h>
#include <signal.h>

void sighandler(int s);

t_log *logger;
socketsT sockets;

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
    iniciarConexiones(entradasalidaConfig,&sockets);
    
    //Testing Send message
    int a;
    while (1)
    {
        scanf("%d", &a); // Esta para bloquear el programa
        send_test(sockets.memoriaSocket, "Perro", 10);
        send_test(sockets.kernelSocket, "Gato", 1);
    }

    //Finalizar
    fin_conexion(logger,&sockets);
    return 0;
}


void sighandler(int s) {
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    fin_conexion(logger,&sockets);
    exit(0);
}