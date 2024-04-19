#include <stdio.h>
#include <utils/config.h>
#include <memoria_config.h>
#include <memoria_conexion.h>
#include "sockets/networking.h"
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
    // Registro de un mensaje en el logger indicando que se está iniciando la memoria
    log_info(logger, "Iniciando memoria");

    // Verificación de que se haya especificado un archivo de configuración al ejecutar el programa
    printf("argc: %d\n", argc);
    if (argc == 1) {
        // Si no se especificó un archivo de configuración, se registra un error en el logger y se termina el programa
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    // Carga de los datos de configuración desde el archivo especificado
    memoria_config_t* memoriaConfig = memoriaConfigLoad(argv[1]);

    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones(memoriaConfig);

    // Escucha de conexiones entrantes hasta que no se establezcan más conexiones
    while(server_escuchar("Servidor Memoria"));

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
    return 0;
}

// Función que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s) {
    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
    // Terminación del programa
    exit(0);
}
