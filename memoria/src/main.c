#include <stdlib.h>
#include <stdio.h>
#include <utils/config.h>
#include <sockets/server.h>
#include <memoria_config.h>

t_log *logger;

int main(int argc, char* argv[]) {
    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando memoria");

    //Obtener datos de configuracion
    memoria_config_t* memoriaConfig = memoriaConfigLoad(argv[1]);

    //Iniciar servidor de memoria
    iniciarServerProceso(memoriaConfig->ipMemoria, memoriaConfig->puertoEscucha, "Memoria");

    //Finalizar
    log_destroy(logger);
    return 0;
}
