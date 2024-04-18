#ifndef TP_2024_1C_GRUPO_NETWORKING_H
#define TP_2024_1C_GRUPO_NETWORKING_H

#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    APROBAR_OPERATIVOS,
    MIRAR_NETFLIX,
    DEBUG_CODE = 69
}op_code_NUESTRO;

///

bool send_aprobar_operativos(int fd, uint8_t  nota1, uint8_t  nota2);
bool recv_aprobar_operativos(int fd, uint8_t* nota1, uint8_t* nota2);

bool send_mirar_netflix(int fd, char*  peli, uint8_t  cant_pochoclos);
bool recv_mirar_netflix(int fd, char** peli, uint8_t* cant_pochoclos);

bool send_debug(int fd);

#endif