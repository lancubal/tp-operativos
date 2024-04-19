//
// Created by utnso on 4/6/24.
//

#include "kernel_user_interface.h"

void kernelUserInterfaceStart(socketsT * sockets) {
    while(true) {
        printf("1. Ejecutar Script de Operaciones\n");
        printf("2. Iniciar proceso\n");
        printf("3. Finalizar proceso\n");
        printf("4. Iniciar planificación\n");
        printf("5. Detener planificación\n");
        printf("6. Listar procesos por estado\n");
        char* comando = readline("> ");
        if(string_is_empty(comando)) {
            break;
        }
    }
}