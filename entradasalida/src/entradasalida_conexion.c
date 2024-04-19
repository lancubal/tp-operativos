//
// Created by utnso on 4/14/24.
//

#include "entradasalida_conexion.h"
#include "pthread.h"

void iniciarConexiones(entradasalida_config_t * entradasalidaConfig, socketsT * sockets){
    sockets->memoriaSocket = connectToServer(entradasalidaConfig->ipMemoria,entradasalidaConfig->puertoMemoria);
    sockets->kernelSocket = connectToServer(entradasalidaConfig->ipKernel,entradasalidaConfig->puertoKernel);
}

void fin_conexion(t_log* logger, socketsT * sockets){
    disconnectClient(sockets->memoriaSocket);
    disconnectClient(sockets->kernelSocket);
    log_info(logger,"Terminado I/O");
    log_destroy(logger);
}