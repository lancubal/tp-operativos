//
// Created by utnso on 4/6/24.
//

#include "cpu_server.h"

// funciona iniciar dispatch e interrupt
void iniciarDispatch(char* ipCPU, char* puertoEscuchaDispatch) {
    //Iniciar servidor de CPU Dispatch
    int socketDispatch = startServer(ipCPU, puertoEscuchaDispatch);
    if(errno != 0) {
        log_error(logger, "Error al iniciar servidor de CPU Dispatch");
        exit(-1);
    }
    log_info(logger, "Servidor de CPU Dispatch iniciado en: %s:%s", ipCPU, puertoEscuchaDispatch);

    // ACA HACE FALTA USAR HILOS PARA QUE SE PUEDA CONECTAR A LOS DOS SERVIDORES AL MISMO TIEMPO
    //Esperar a que se conecte el Kernel por Dispatch
    log_info(logger, "Esperando al Kernel por Dispatch");
    int socketKernel = waitClient(socketDispatch);
    if(errno == 0) {
        log_error(logger, "Error al conectar al Kernel por Dispatch");
        exit(-1);
    }
}

void iniciarInterrupt(char* ipCPU, char* puertoEscuchaInterrupt) {
//Iniciar servidor de CPU Interrupt
    int socketInterrupt = startServer(ipCPU, puertoEscuchaInterrupt);
    if(errno != 0) {
        log_error(logger, "Error al iniciar servidor de CPU Interrupt");
        exit(-1);
    }
    log_info(logger, "Servidor de CPU Interrupt iniciado en: %s:%s", ipCPU, puertoEscuchaInterrupt);

    // ACA HACE FALTA USAR HILOS PARA QUE SE PUEDA CONECTAR A LOS DOS SERVIDORES AL MISMO TIEMPO
    //Esperar a que se conecte el Kernel por Interrupt
    log_info(logger, "Esperando al Kernel por Interrupt");
    int socketKernel = waitClient(socketInterrupt);
    if(errno == 0) {
        log_error(logger, "Error al conectar al Kernel por Interrupt");
        exit(-1);
    }
}