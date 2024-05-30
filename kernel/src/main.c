#include <utils/config.h>
#include <kernel_config.h>
#include "kernel_user_interface.h"
#include <kernel_conexion.h>
#include <signal.h>
#include <planificador.h>
#include <commons/collections/queue.h>

// Definición de la función sighandler que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s);

/*          VARIABLES GLOBALES          */
// Declaración del logger que se utilizará para registrar los eventos del programa
t_log* logger = NULL;
// Declaración de la estructura kernel_config_t que almacenará los datos de configuración del kernel
kernel_config_t * kernel_config = NULL;
// Declaración de la estructura socketsT que almacenará los sockets utilizados en el programa
socketsT* sockets = NULL;
// Semaforos
sem_t sem_ready_queue;
sem_t sem_new_queue;
sem_t sem_pcb;
// Colas
t_queue* ready_queue = NULL;
//t_queue* new_queue = NULL;
// PCB en ejecución
t_PCB * pcb = NULL;

// Función principal del programa
int main(int argc, char* argv[]) {
    // Manejo de la señales
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGSEGV, sighandler);

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
    kernelConfigLoad(argv[1]);

    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones();

    // Creación de un nuevo hilo para escuchar conexiones entrantes
    pthread_t kernel_thread;
    pthread_create(&kernel_thread,NULL,(void *)phread_server_escuchar,&sockets->kernelSocket);

    // Inicio de la interfaz de usuario del kernel
    //kernelUserInterfaceStart(&sockets);

    // Inicio del planificador
    // Push to ready_queue 5 random PCB
    ready_queue = queue_create();
    T_CPU_REGISTERS* registers;
    for (int i = 1; i <= 5; i++) {
        registers = init_CPU_REGISTERS(i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        queue_push(ready_queue, init_PCB(i, 99, "NEW", registers, 100));
    }
    // Creación del hilo para el planificador
    pthread_t short_term_scheduler_thread;
    pthread_create(&short_term_scheduler_thread, NULL, (void *) SHORT_TERM_SCHEDULER, NULL);
    // Creacion de un nuevo hilo para escuchar a los servidores conectados
    pthread_t dispatcher_thread;
    pthread_create(&dispatcher_thread, NULL, (void *) cliente_escuchar, &sockets->dispatchSocket);
    pthread_t interrupt_thread;
    pthread_create(&interrupt_thread, NULL, (void *) cliente_escuchar, &sockets->interruptSocket);
    pthread_t memory_thread;
    pthread_create(&memory_thread, NULL, (void *) cliente_escuchar, &sockets->memoriaSocket);

    pthread_join(kernel_thread, NULL);
    pthread_join(dispatcher_thread, NULL);
    pthread_join(interrupt_thread, NULL);
    pthread_join(memory_thread, NULL);
    pthread_join(short_term_scheduler_thread, NULL);

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
    // Cierre del hilo de escucha de conexiones
    close(kernel_thread);
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