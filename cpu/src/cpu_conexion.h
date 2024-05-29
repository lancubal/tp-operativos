//
// Created by utnso on 4/14/24.
//

#ifndef TP_2024_1C_GRUPO_CPU_CONEXION_H
#define TP_2024_1C_GRUPO_CPU_CONEXION_H


#include <cpu_config.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <commons/log.h>
#include <pthread.h>
#include <sockets/server.h>
#include <sockets/client.h>
#include "sockets/protocol.h"
#include <semaphore.h>

// Definimos una estructura para almacenar los argumentos necesarios para procesar una conexión.
// Esta estructura contiene el descriptor de archivo del socket y el nombre del servidor.
typedef struct {
    int fd;             // Descriptor de archivo del socket
    char* server_name;  // Nombre del servidor
} t_procesar_conexion_args;

extern T_CPU_REGISTERS CPU_Registers;
extern sem_t sem_pcb;
extern sem_t sem_cycle;

void iniciarConexiones(cpu_config_t* cpuConfig);
int server_escuchar(char* server_name, const int*  server_socket);
int phread_server_escuchar(void* server_socket);
void procesar_conexion(t_procesar_conexion_args*);
void fin_conexion();
#endif //TP_2024_1C_GRUPO_CPU_CONEXION_H
