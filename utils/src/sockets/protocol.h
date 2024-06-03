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


// Codigos de CPU comienzan en 1XX, codigos de memoria en 2XX, codigos de kernel en 3XX, codigos de io en 4XX
typedef enum {
    // CPU
    FETCH = 100,
    // MEMORIA
    NEW_PROCESS = 211,
    END_PROCESS = 212,
    WRITE_FROM_PHYSICAL = 240,
    READ_FROM_PHYSICAL = 241,
    // KERNEL
    PCB = 310,
    WAIT = 311,
    SIGNAL = 312,
    IO_END = 340,
    ERROR_OP = -1,
    // I/O
    IO_GEN_SLEEP = 400,
    IO_STDIN_READ = 401,
    IO_STDOUT_WRITE = 402,
    IO_FS_CREATE = 403,
    IO_FS_DELETE = 404,
    IO_FS_TRUNCATE = 405,
    IO_FS_WRITE = 406,
    IO_FS_READ = 407
} OP_CODES;

typedef struct {
    // Header
    OP_CODES op_code;
    size_t payload_size;
    // Payload
    char* payload;
} t_packet;

// Serializers and deserializers
typedef void (*serializer)(void *data, char **buffer, size_t *size);
void serialize_packet(void* packet, char** buffer, size_t *size);
void deserialize_packet(char* buffer, size_t size, t_packet* packet);
void serialize_pcb(void* pcb, char** buffer, size_t *size);
void deserialize_pcb(char* buffer, size_t size, t_PCB* pcb);

// Packet management
t_packet* create_packet(OP_CODES op_code, size_t payload_size, void* data, serializer serializer_func);
bool destroy_packet(t_packet* packet);
bool send_packet(int fd, t_packet* packet);
bool recv_packet(int fd, t_packet* packet);

#endif