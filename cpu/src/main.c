#include <cpu_config.h>
#include <commons/log.h>
#include <cpu_conexion.h>
#include <utils/module_tads.h>
#include <sockets/conexiontad.h>
#include <cpu_ciclo.h>
#include <signal.h>

void sighandler(int s) {
    log_info(logger,"Terminado el Servidor CPU");
    log_destroy(logger);
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    exit(0);
}

t_log* logger;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);
    socketsT sockets;

    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando CPU");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    cpu_config_t* cpuConfig = cpuConfigLoad(argv[1]);
    sockets.dispatchSocket = iniciarServerProceso(cpuConfig->ipMemoria, cpuConfig->puertoMemoria, "Memoria");
    sockets.dispatchSocket = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaDispatch, "CPU Dispatch");
    sockets.interruptSocket = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaInterrupt, "CPU Interrupt");

    pthread_t dispatch_thread;
    pthread_create(&dispatch_thread,NULL,(void *)phread_server_escuchar,&sockets.dispatchSocket);
    pthread_t interrupt_thread;
    pthread_create(&interrupt_thread,NULL,(void *)phread_server_escuchar,&sockets.interruptSocket);

    int fin;
    pthread_join(dispatch_thread,(void*) &fin);
    pthread_join(interrupt_thread,(void*) &fin);
    // //Iniciar conexiones
    // socketsT* sockets = iniciarConexiones(cpuConfig);

    // //Pedir siguiente instruccion a la memoria
    // registroCPU* registro = malloc(sizeof(registroCPU));

    // char* instruccion = fetch(registro->PC, sockets->memoriaSocket);
    // log_info(logger, "Instruccion obtenida: %s", instruccion);

    //Finalizar
    log_destroy(logger);
    return 0;
}