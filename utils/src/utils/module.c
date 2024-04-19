//
// Created by utnso on 4/5/24.
//

#include "module.h"

void moduleTerminate(int conexion, t_log* logger, t_config* config)
{

    log_destroy(logger);
    config_destroy(config);
}