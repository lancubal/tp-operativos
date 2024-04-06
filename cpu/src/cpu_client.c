//
// Created by utnso on 4/6/24.
//

#include "cpu_client.h"

void conectarAMemoria(char* ipMemoria, char* puertoMemoria) {
    int socketMemoria = connectToServer(ipMemoria, puertoMemoria);
    if(errno != 0) {
    log_error(logger, "Error al conectar a memoria");
        exit(-1);
    }
    log_info(logger, "Conectado a memoria en: %s:%s", ipMemoria, puertoMemoria);
}