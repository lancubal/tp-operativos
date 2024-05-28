#include <cpu_config.h>
#include <commons/log.h>
#include <cpu_conexion.h>
#include <cpu_ciclo.h>
#include <signal.h>

// Declaración de la estructura socketsT que almacenará los sockets utilizados en el programa
socketsT sockets;

// Declaración del logger que se utilizará para registrar los eventos del programa
t_log *logger;

// Definición de la función sighandler que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s);

T_CPU_REGISTERS* CPU_Registers;

sem_t sem_pcb;

// Función principal del programa
int main(int argc, char* argv[]) {
    // Manejo de la señales
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGSEGV, sighandler);

    // Inicialización del logger
    logger = loggerCreate();
    // Registro de un mensaje en el logger indicando que se está iniciando la CPU
    log_info(logger, "Iniciando CPU");

    // Verificación de que se haya especificado un archivo de configuración al ejecutar el programa
    if (argc < 2) {
        // Si no se especificó un archivo de configuración, se registra un error en el logger y se termina el programa
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    // Carga de los datos de configuración desde el archivo especificado
    cpu_config_t* cpuConfig = cpuConfigLoad(argv[1]);

    // Inicializo los registros de la CPU
    CPU_Registers = malloc(sizeof(T_CPU_REGISTERS));

    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones(cpuConfig);

    // Creación de hilos para escuchar a los clientes
    pthread_t dispatch_thread;
    pthread_create(&dispatch_thread,NULL,(void *)phread_server_escuchar,&sockets.dispatchSocket);
    pthread_t interrupt_thread;
    pthread_create(&interrupt_thread,NULL,(void *)phread_server_escuchar,&sockets.interruptSocket);

    cpu_ciclo();

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion(&sockets);
    // Cierre de los hilos de escucha de conexiones
    close(dispatch_thread);
    close(interrupt_thread);
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