#include <stdlib.h>
#include <stdio.h>
#include <utils/logger.h>
#include <sockets/server.h>

int main(int argc, char* argv[]) {
    log_info(logger, "Iniciando kernel");
    t_config* config = getConfig(argv[1]);
    startServer("127.0.0.1", "8003");
    log_destroy(logger);
    return 0;
}
