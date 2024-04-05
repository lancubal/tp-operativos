#include <stdlib.h>
#include <stdio.h>
#include <utils/config.h>
#include <sockets/server.h>
#include <utils/logger.h>

int main(int argc, char* argv[]) {
    logger = loggerCreate();
    log_info(logger, "Iniciando kernel");
    t_config* config = getConfig(argv[1]);
    startServer("127.0.0.1", "8003");
    while(true) {
        printf("1. Ejecutar Script de Operaciones\n");
        printf("2. Iniciar proceso\n");
        printf("3. Finalizar proceso\n");
        printf("4. Iniciar planificación\n");
        printf("5. Detener planificación\n");
        printf("6. Listar procesos por estado\n");
        char* comando = readline("> ");
        if(string_is_empty(comando)) {
            break;
        }
    }
    log_destroy(logger);
    return 0;
}
