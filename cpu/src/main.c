#include <cpu_config.h>
#include <commons/log.h>
#include <cpu_conexion.h>
#include <utils/module_tads.h>
#include <sockets/conexiontad.h>
#include "sockets/client.h"
#include <cpu_ciclo.h>
#include <signal.h>


void sighandler(int s);

socketsT sockets;
t_log* logger;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);
    

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
    iniciarConexiones(cpuConfig,&sockets);

    pthread_t dispatch_thread;
    pthread_create(&dispatch_thread,NULL,(void *)phread_server_escuchar,&sockets.dispatchSocket);
    pthread_t interrupt_thread;
    pthread_create(&interrupt_thread,NULL,(void *)phread_server_escuchar,&sockets.interruptSocket);

    //Testing Send message
    int a;
    while (1)
    {
        scanf("%d", &a); // Esta para bloquear el programa
        send_test(sockets.memoriaSocket, "Perro", 14);
    }
    

    // int fin; // Mas adelante cambiarlo por socket de Kernel
    // pthread_join(dispatch_thread,(void*) &fin);  // Deberia recibir señal para cerrar el thread
    // pthread_join(interrupt_thread,(void*) &fin); // Deberia recibir señal para cerrar el thread
 

    //Finalizar
    fin_conexion(logger,&sockets);
    close(dispatch_thread);
    close(interrupt_thread);
    return 0;
}



void sighandler(int s) {
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    fin_conexion(logger,&sockets);
    exit(0);
}