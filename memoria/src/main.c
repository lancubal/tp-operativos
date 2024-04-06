#include <stdlib.h>
#include <stdio.h>
#include <utils/config.h>
#include <sockets/server.h>

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
    t_config* config = getConfig(argv[1]);
    char* ipMemoria = config_get_string_value(config, "IP_MEMORIA");
    char* puertoEscucha = string_from_format("%d", config_get_int_value(config, "PUERTO_ESCUCHA"));
    int tamMemoria = config_get_int_value(config, "TAM_MEMORIA");
    int tamPagina = config_get_int_value(config, "TAM_PAGINA");
    char* pathInstrucciones = config_get_string_value(config, "PATH_INSTRUCCIONES");
    int retardoRespuesta = config_get_int_value(config, "RETARDO_RESPUESTA");
    log_info(logger, "Configuracion cargada");

    //Iniciar servidor de memoria
    int socketMemoria = startServer(ipMemoria, puertoEscucha);
    if(socketMemoria == -1) {
        log_error(logger, "Error al iniciar servidor de memoria");
        return -1;
    }
    log_info(logger, "Servidor de memoria iniciado en: %s:%s", ipMemoria, puertoEscucha);

    //Finalizar
    config_destroy(config);
    log_destroy(logger);
    return 0;
}
