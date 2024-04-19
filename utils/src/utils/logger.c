//
// Created by utnso on 4/5/24.
//

#include "logger.h"

/**
 * @brief Lee desde la consola y registra la entrada en el logger.
 *
 * Esta función lee líneas desde la consola hasta que se introduce una línea vacía.
 * Cada línea leída se registra en el logger proporcionado.
 *
 * @param logger Un puntero al logger donde se registrarán las líneas leídas.
 */
void readFromConsole(t_log* logger)
{
    // Declara una variable para almacenar la línea leída
    char* leido;

    // Entra en un bucle infinito
    while(true) {
        // Lee una línea desde la consola
        leido = readline("> ");

        // Si la línea leída es vacía, rompe el bucle
        if (string_is_empty(leido)) {
            break;
        }

        // Registra la línea leída en el logger
        log_info(logger, "%s", leido);
    }

    // Libera la memoria asignada para la línea leída
    free(leido);
}

/**
 * @brief Crea un nuevo logger.
 *
 * Esta función crea un nuevo logger que escribe en el archivo "tp.log" con el nombre de programa "client".
 * El logger creado tiene un nivel de logueo de INFO y se muestra por consola.
 *
 * @return Un puntero al logger creado.
 */
t_log* loggerCreate(void)
{
    // Crea un nuevo logger
    t_log* nuevo_logger = log_create("tp.log", "client", 1, LOG_LEVEL_INFO);

    // Devuelve el logger creado
    return nuevo_logger;
}
