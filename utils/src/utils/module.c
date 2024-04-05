//
// Created by utnso on 4/5/24.
//

#include "module.h"

void moduleTerminate(int conexion, t_log* logger, t_config* config)
{
    /* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
      con las funciones de las commons y del TP mencionadas en el enunciado */
    log_destroy(logger);
    config_destroy(config);
}