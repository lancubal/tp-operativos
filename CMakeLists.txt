cmake_minimum_required(VERSION 3.27)
project(tp_2024_1c_grupo)

set(CMAKE_CXX_STANDARD 14)

include_directories(utils/src/)

add_executable(tp_2024_1c_grupo
        cpu/src/main.c
        entradasalida/src/main.c
        kernel/src/main.c
        memoria/src/main.c
        memoria/settings.mk
        utils/src/sockets/client.c
        utils/src/sockets/client.h
        utils/src/sockets/server.c
        utils/src/sockets/server.h
        utils/src/utils/config.c
        utils/src/utils/config.h
        utils/src/utils/logger.c
        utils/src/utils/logger.h
        utils/src/utils/module.c
        utils/src/utils/module.h)
