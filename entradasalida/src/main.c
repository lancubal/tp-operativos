#include <utils/logger.h>
#include <utils/config.h>
#include "sockets/client.h"
#include "sockets/protocol.h"
#include <entradasalida_config.h>
#include <entradasalida_conexion.h>
#include <signal.h>

// Definición de la función sighandler que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s);

// Declaración del logger que se utilizará para registrar los eventos del programa
t_log *logger;

// Declaración de la estructura socketsT que almacenará los sockets utilizados en el programa
socketsT sockets;

// Función principal del programa
int main(int argc, char* argv[]) {
    // Manejo de la señales
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGSEGV, sighandler);

    // Inicialización del logger
    logger = loggerCreate();
    // Registro de un mensaje en el logger indicando que se está iniciando la entrada/salida
    log_info(logger, "Iniciando I/O");

    // Verificación de que se haya especificado un archivo de configuración al ejecutar el programa
    if (argc < 2) {
        // Si no se especificó un archivo de configuración, se registra un error en el logger y se termina el programa
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    // Carga de los datos de configuración desde el archivo especificado
    entradasalida_config_t * entradasalidaConfig = entradasalidaConfigLoad(argv[1]);
    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones(entradasalidaConfig);

    // Prueba de envío de mensajes
    int a;
    while (1)
    {
        // Bloqueo del programa para esperar la entrada del usuario
        scanf("%d", &a);
        // Envío de un mensaje de prueba al socket de memoria
        send_test(sockets.memoriaSocket, "Perro", 10);
        // Envío de un mensaje de prueba al socket del kernel
        send_test(sockets.kernelSocket, "Gato", 1);
    }

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion(logger,&sockets);
    return 0;
}

// Función que se ejecutará cuando se reciba una señal
void sighandler(int signal) {
    signal == SIGINT ? log_warning(logger, "Se ha recibido la señal %s", strsignal(signal)) :
    log_error(logger, "Se ha recibido la señal %s", strsignal(signal));
    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
    // Terminación del programa
    exit(0);
}