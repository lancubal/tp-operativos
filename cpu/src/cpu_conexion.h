//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_CPU_CONEXION_H
#define TP_2024_1C_GRUPO_CPU_CONEXION_H

#include <sockets/client.h>
#include <sockets/server.h>
#include <cpu_config.h>
#include <pthread.h>

void iniciarConexiones(cpu_config_t* cpuConfig);

#endif //TP_2024_1C_GRUPO_CPU_CONEXION_H
