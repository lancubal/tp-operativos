#include <stdio.h>
#include <utils/config.h>
#include <memoria_config.h>
#include <memoria_conexion.h>

t_log *logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando memoria");

    //Verificar que haya un archivo de configuración
    printf("argc: %d\n", argc);
    if (argc == 1) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    memoria_config_t* memoriaConfig = memoriaConfigLoad(argv[1]);

    //Iniciar conexiones
    iniciarConexiones(memoriaConfig);

    //Finalizar
    //disconnectServer(socketCPU);
    log_destroy(logger);
    return 0;
}
