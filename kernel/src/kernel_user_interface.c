//
// Created by utnso on 4/6/24.
//

#include "kernel_user_interface.h"

/**
 * @brief Inicia la interfaz de usuario del Kernel.
 *
 * Esta función se encarga de iniciar la interfaz de usuario del Kernel.
 * Muestra un menú con varias opciones, como ejecutar un script de operaciones, iniciar un proceso,
 * finalizar un proceso, iniciar la planificación, detener la planificación y listar los procesos por estado.
 * El usuario puede seleccionar una opción ingresando el número correspondiente.
 * La función continúa ejecutándose hasta que el usuario ingresa una cadena vacía, momento en el que se detiene.
 *
 * @param sockets Una estructura de sockets que contiene los descriptores de archivo de las conexiones.
 * @return 0 cuando se sale del bucle.
 */
int kernelUserInterfaceStart(socketsT * sockets) {
    // Entramos en un bucle donde mostramos el menú y esperamos la entrada del usuario
    while(true) {
        // Mostramos el menú
        printf("1. Ejecutar Script de Operaciones\n");
        printf("2. Iniciar proceso\n");
        printf("3. Finalizar proceso\n");
        printf("4. Iniciar planificación\n");
        printf("5. Detener planificación\n");
        printf("6. Listar procesos por estado\n");
        // Esperamos la entrada del usuario
        char* comando = readline("> ");
        // Si el usuario ingresó una cadena vacía, salimos del bucle
        if(string_is_empty(comando)) {
            break;
        }
    }
    // Retornamos 0 cuando salimos del bucle
    return 0;
}