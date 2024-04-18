#include <cpu_config.h>
#include <commons/log.h>
#include <cpu_conexion.h>
#include <utils/module_tads.h>
#include <sockets/conexiontad.h>
#include <cpu_ciclo.h>
#include <signal.h>

void sighandler(int s) {
    log_destroy(logger);
    // Agregar cualquier funcion luego de que el programa reciba la señal del "CTRL + C"
    exit(0);
}

t_log* logger;

int main(int argc, char* argv[]) {
    signal(SIGINT, sighandler);
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
    socketsT* sockets = iniciarConexiones(cpuConfig);

    //Pedir siguiente instruccion a la memoria
    registroCPU* registro = malloc(sizeof(registroCPU));

    char* instruccion = fetch(registro->PC, sockets->memoriaSocket);
    log_info(logger, "Instruccion obtenida: %s", instruccion);

    //Finalizar
    log_destroy(logger);
    return 0;
}