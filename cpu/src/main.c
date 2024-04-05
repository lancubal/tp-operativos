#include <stdlib.h>
#include <stdio.h>
#include <utils/config.h>
#include <sockets/server.h>
#include <utils/logger.h>

int main(int argc, char* argv[]) {
    logger = loggerCreate();
    log_info(logger, "Iniciando CPU");
    t_config* config = getConfig(argv[1]);
    startServer("127.0.0.1", "8002");
    log_destroy(logger);
    return 0;
}
