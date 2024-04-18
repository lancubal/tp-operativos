
#include <utils/config.h>
#include <kernel_config.h>
#include "kernel_user_interface.h"
#include <kernel_conexion.h>
#include <signal.h>

void sighandler(int s) {
    log_destroy(logger);
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    exit(0);
}

t_log *logger;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);

    //Iniciar logger
    logger = loggerCreate();
    log_info(logger, "Iniciando kernel");

    //Verificar que haya un archivo de configuración
    if (argc < 2) {
        log_error(logger, "No se ha especificado un archivo de configuración");
        return 1;
    }

    //Obtener datos de configuracion
    kernel_config_t * kernelConfig = kernelConfigLoad(argv[1]);

    //Iniciar conexiones
    iniciarConexiones(kernelConfig);

    //Consola interactiva
    kernelUserInterfaceStart();

    //Finalizar
    log_destroy(logger);
    return 0;
}
