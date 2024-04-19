
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

    sockets.kernelSocket = iniciarServerProceso(kernelConfig->ipKernel, kernelConfig->puertoEscucha, "Kernel Server");
    sockets.memoriaSocket = connectToServer(kernelConfig->ipMemoria,kernelConfig->puertoMemoria);
    sockets.dispatchSocket = connectToServer(kernelConfig->ipCPU,kernelConfig->puertoCPUDispatch);
    sockets.interruptSocket = connectToServer(kernelConfig->ipCPU,kernelConfig->puertoCPUInterrupt);

    kernelUserInterfaceStart(&sockets);


    //Finalizar
    disconnectServer(sockets.kernelSocket);
    disconnectClient(sockets.memoriaSocket);
    disconnectClient(sockets.dispatchSocket);
    disconnectClient(sockets.interruptSocket);
    log_destroy(logger);
    return 0;
}


void sighandler(int s) {
    disconnectServer(sockets.kernelSocket);
    disconnectClient(sockets.memoriaSocket);
    disconnectClient(sockets.dispatchSocket);
    disconnectClient(sockets.interruptSocket);
    log_info(logger,"Terminado el Servidor CPU");
    log_destroy(logger);
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    exit(0);
}