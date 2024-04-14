#include <cpu_config.h>
#include <commons/log.h>
#include <cpu_conexion.h>

t_log* logger;

int main(int argc, char* argv[]) {
    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando CPU");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    cpu_config_t* cpuConfig = cpuConfigLoad(argv[1]);

    //Iniciar conexiones
    iniciarConexiones(cpuConfig);

    //Finalizar
    log_destroy(logger);
    return 0;
}