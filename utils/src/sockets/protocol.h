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
#include <utils/module_tads.h>
#include <utils/logger.h>

typedef enum {
    TEST = 10,
    PCB = 20,
    DEBUG_CODE = 69,
    ERROR_OP = -1 
} OP_CODES;

bool send_test(int fd, char*  cadena, uint8_t  cant);
bool recv_test(int fd, char** cadena, uint8_t* cant);

void* serializer(void *tad, const size_t *size);
void* deserializer(void *stream, const size_t *size);
bool send_tad(int fd, void* tad, const size_t *size);
bool recv_tad(int fd, void** tad);

bool send_opcode(int fd, OP_CODES op);
bool recv_opcode(int fd, OP_CODES* op);



bool send_debug(int fd);

#endif