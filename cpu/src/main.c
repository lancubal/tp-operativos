#include <cpu_config.h>
#include <commons/log.h>
#include <cpu_conexion.h>
#include <cpu_ciclo.h>
#include <signal.h>

// Definición de la función sighandler que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s);

/*         VARIABLES GLOBALES         */
// Declaración del logger que se utilizará para registrar los eventos del programa
t_log *logger;
// Declaración de la estructura cpu_config_t que almacenará los datos de configuración de la CPU
cpu_config_t *cpu_config = NULL;
// Declaración de la estructura socketsT que almacenará los sockets utilizados en el programa
socketsT* sockets;
// PCB en ejecución
t_PCB *pcb = NULL;
// Instruccion fetcheada
char* instruccion = NULL;
// Instruccion decodificada
instruction_decoded_t* decoded_instruction;
// Interrupcion
bool interrupcion = false;

// Semaforos
sem_t sem_pcb;
sem_t sem_cycle;
sem_t sem_instruccion;


// Función principal del programa
int main(int argc, char* argv[]) {
    // Manejo de la señales
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGSEGV, sighandler);

    // Inicialización de los semaforos
    sem_init(&sem_pcb, 0, 0);
    sem_init(&sem_cycle, 0, 0);
    sem_init(&sem_instruccion, 0, 0);

    // Inicializar PCB
    pcb = malloc(sizeof(t_PCB));

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
    cpuConfigLoad(argv[1]);

    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones(cpu_config);

    // Creación de hilos para escuchar a los clientes
    pthread_t dispatch_thread;
    pthread_create(&dispatch_thread,NULL,(void *)phread_server_escuchar,&sockets->dispatchSocket);
    pthread_t interrupt_thread;
    pthread_create(&interrupt_thread,NULL,(void *)phread_server_escuchar,&sockets->interruptSocket);

    // Creacion de un nuevo hilo para escuchar a los servidores conectados
    pthread_t memoria_thread;
    pthread_create(&memoria_thread,NULL,(void *)cliente_escuchar,&sockets->memoriaSocket);

    // Creacion de hilo para el ciclo de la cpu
    pthread_t cpu_ciclo_thread;
    pthread_create(&cpu_ciclo_thread,NULL,(void *)cpu_ciclo,NULL);

    pthread_join(dispatch_thread,NULL);
    pthread_join(interrupt_thread,NULL);
    pthread_join(memoria_thread,NULL);

    pthread_join(cpu_ciclo_thread,NULL);

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion(&sockets);
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