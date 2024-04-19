
#include <utils/config.h>
#include <kernel_config.h>
#include "kernel_user_interface.h"
#include <kernel_conexion.h>
#include <signal.h>


// Definición de la función sighandler que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s);

// Declaración del logger que se utilizará para registrar los eventos del programa
t_log *logger;

// Declaración de la estructura socketsT que almacenará los sockets utilizados en el programa
socketsT sockets;

// Función principal del programa
int main(int argc, char* argv[]) {
    // Asignación de la función sighandler para manejar la señal SIGINT (CTRL + C)
    signal(SIGINT, sighandler);

    // Inicialización del logger
    logger = loggerCreate();
    // Registro de un mensaje en el logger indicando que se está iniciando el kernel
    log_info(logger, "Iniciando kernel");

    // Verificación de que se haya especificado un archivo de configuración al ejecutar el programa
    if (argc < 2) {
        // Si no se especificó un archivo de configuración, se registra un error en el logger y se termina el programa
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    // Carga de los datos de configuración desde el archivo especificado
    kernel_config_t * kernelConfig = kernelConfigLoad(argv[1]);

    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones(kernelConfig);

    // Creación de un nuevo hilo para escuchar conexiones entrantes
    pthread_t kernel_thread;
    pthread_create(&kernel_thread,NULL,(void *)phread_server_escuchar,&sockets.kernelSocket);

    // Inicio de la interfaz de usuario del kernel
    kernelUserInterfaceStart(&sockets);

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
    // Cierre del hilo de escucha de conexiones
    close(kernel_thread);
    return 0;
}

// Función que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s) {
    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
    // Terminación del programa
    exit(0);
}