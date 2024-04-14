#include <utils/logger.h>
#include <utils/config.h>
#include "sockets/client.h"
#include <entradasalida_config.h>
#include <entradasalida_conexion.h>

t_log* logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando I/O");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    entradasalida_config_t * entradasalidaConfig = entradasalidaConfigLoad(argv[1]);

    //Iniciar conexiones
    iniciarConexiones(entradasalidaConfig);

    //Finalizar
    log_destroy(logger);
    return 0;
}
