//
// Created by utnso on 5/30/24.
//

#include "entradasalida_interfaces.h"

void start_interface() {
    t_interface* interface = malloc(sizeof(t_interface));
    // Inicializamos la interfaz
    if (strcmp(entradasalida_config->interface_type, "STDIN") == 0) {
        // Inicializamos la interfaz STDIN
        interface->type = STDIN;
        interface->name = "STDIN";
        log_info(logger, "Inicializando interfaz %s", interface->name);
    } else if (strcmp(entradasalida_config->interface_type, "STDOUT") == 0) {
        // Inicializamos la interfaz STDOUT
        interface->type = STRDOUT;
        interface->name = "STDOUT";
        log_info(logger, "Inicializando interfaz %s", interface->name);
    } else if (strcmp(entradasalida_config->interface_type, "DIALFS") == 0) {
        // Inicializamos la interfaz DIALFS
        interface->type = DIALFS;
        interface->name = "DIALFS";
        log_info(logger, "Inicializando interfaz %s", interface->name);
    } else {
        // Inicializamos la interfaz genérica
        interface->type = GENERIC;
        interface->name = "GENERIC";
        log_info(logger, "Inicializando interfaz %s", interface->name);
    }
    // Liberamos la memoria utilizada por la interfaz
    free(interface);
}