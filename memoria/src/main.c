#include <stdio.h>
#include <utils/config.h>
#include <memoria_config.h>
#include <memoria_conexion.h>
#include <signal.h>

// Definición de la función sighandler que se ejecutará cuando se reciba la señal SIGINT (CTRL + C)
void sighandler(int s);

/*          VARIABLES GLOBALES          */
// Declaración del logger que se utilizará para registrar los eventos del programa
t_log *logger = NULL;
// Declaración de la estructura memoria_config_t que almacenará los datos de configuración de la memoria
memoria_config_t* memoria_config = NULL;
// Declaración de la estructura socketsT que almacenará los sockets utilizados en el programa
socketsT* sockets = NULL;
// Memoria contigua
void* memory;
// Tabla de páginas del proceso actual (simulación simple con una única tabla)
t_page_table page_table;


// Instrucciones de testeo
char* instrucciones[] = {
        "SET AX 1",
        "SET BX 1",
        "SET PC 5",
        "SUM AX BX",
        "SUB AX BX",
        "MOV IN EDX ECX",
        "MOV_OUT EDX ECX",
        "RESIZE 128",
        "JNZ AX 4",
        "COPY_STRING 8",
        "IO_GEN_SLEEP Int1 10",
        "IO_STDIN_READ Int2 EAX AX",
        "IO_STDOUT_WRITE Int3 BX EAX",
        "IO_FS_CREATE Int4 notas.txt 15 IO_FS_DELETE Int4 notas.txt",
        "IO_FS_TRUNCATE Int4 notas.txt ECX",
        "IO_FS_WRITE Int4 notas.txt AX ECX EDX",
        "IO_FS_READ Int4 notas.txt BX ECX EDX",
        "WAIT RECURSO_1",
        "SIGNAL RECURSO_1",
        "EXIT"
};

// Función principal del programa
int main(int argc, char* argv[]) {
    // Manejo de la señales
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    signal(SIGSEGV, sighandler);

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
    memoriaConfigLoad(argv[1]);

    // Inicio de las conexiones utilizando los datos de configuración cargados
    iniciarConexiones();

    // Inicialización de la memoria
    init_memory();

    // Escucha de conexiones entrantes hasta que no se establezcan más conexiones
    while(server_escuchar("Servidor Memoria"));

    // Finalización de todas las conexiones y liberación de los recursos utilizados
    fin_conexion();
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
