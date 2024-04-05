//
// Created by utnso on 4/5/24.
//

#include "logger.h"

void readFromConsole(t_log* logger)
{
    char* leido;

    // La primera te la dejo de yapa

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while(true) {
        leido = readline("> ");
        if (string_is_empty(leido)) {
            break;
        }

        log_info(logger, "%s", leido);
    }
    // ¡No te olvides de liberar las lineas antes de regresar!
    free(leido);
}

t_log* loggerCreate(void)
{
    t_log* nuevo_logger = log_create("tp.log", "client", 1, LOG_LEVEL_INFO);

    return nuevo_logger;
}
