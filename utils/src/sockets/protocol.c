#include <commons/log.h>
#include <errno.h>
#include "protocol.h"

// Serializers and deserializers
void serialize_packet(void* data, char** buffer, size_t *size) {
    t_packet *packet = (t_packet *) data; // Convertir el puntero genérico a un puntero de paquete
    // Asignar memoria para el buffer
    *size = sizeof(OP_CODES) + sizeof(size_t) + packet->payload_size;
    *buffer = malloc(*size);
    if (*buffer == NULL) {
        log_error(logger, "Error al asignar memoria para el buffer: %s", strerror(errno));
        return;
    }

    // Copiar los datos del paquete en el buffer
    size_t offset = 0; // Desplazamiento en el buffer
    memcpy(*buffer, &packet->op_code, sizeof(OP_CODES)); // Copiar el código de operación
    offset += sizeof(OP_CODES);
    memcpy(*buffer + sizeof(OP_CODES), &packet->payload_size, sizeof(size_t)); // Copiar el tamaño del payload
    offset += sizeof(size_t);
    memcpy(*buffer + sizeof(OP_CODES) + sizeof(size_t), packet->payload, packet->payload_size); // Copiar el payload
}

void deserialize_packet(char* buffer, size_t size, t_packet* packet) {
    // Copiar los datos del buffer en el paquete
    size_t offset = 0; // Desplazamiento en el buffer
    memcpy(&packet->op_code, buffer, sizeof(OP_CODES)); // Copiar el código de operación
    offset += sizeof(OP_CODES);
    memcpy(&packet->payload_size, buffer + sizeof(OP_CODES), sizeof(size_t)); // Copiar el tamaño del payload
    offset += sizeof(size_t);
    // Asignar memoria para el payload
    packet->payload = malloc(packet->payload_size);
    if (packet->payload == NULL) {
        log_error(logger, "Error al asignar memoria para el payload: %s", strerror(errno));
        return;
    }
    // Copiar los datos del buffer en el payload
    memcpy(packet->payload, buffer + sizeof(OP_CODES) + sizeof(size_t), packet->payload_size);
}

void serialize_pcb(void* data, char** buffer, size_t *size) {
    t_PCB *pcb = (t_PCB *) data; // Convertir el puntero genérico a un puntero de PCB
    // Calcular el tamaño del PCB
    *size = sizeof(pcb->PID) + sizeof(pcb->Quantum) + strlen(pcb->State) + 1 + sizeof(pcb->CPU_REGISTERS);

    // Asignar memoria para el buffer
    *buffer = malloc(*size);
    if (*buffer == NULL) {
        log_error(logger, "Error al asignar memoria para el buffer: %s", strerror(errno));
        return;
    }

    // Copiar los datos del PCB en el buffer
    size_t offset = 0; // Desplazamiento en el buffer
    memcpy(*buffer + offset, &pcb->PID, sizeof(pcb->PID)); // Copiar el PID
    offset += sizeof(pcb->PID);
    memcpy(*buffer + offset, &pcb->Quantum, sizeof(pcb->Quantum)); // Copiar el Quantum
    offset += sizeof(pcb->Quantum);
    memcpy(*buffer + offset, pcb->State, strlen(pcb->State) + 1); // Copiar el Estado
    offset += strlen(pcb->State) + 1;
    memcpy(*buffer + offset, &pcb->CPU_REGISTERS, sizeof(pcb->CPU_REGISTERS)); // Copiar los registros de la CPU
}

void deserialize_pcb(char* buffer, size_t size, t_PCB* pcb) {
    // Copiar los datos del buffer en el PCB
    size_t offset = 0; // Desplazamiento en el buffer
    memcpy(&pcb->PID, buffer + offset, sizeof(pcb->PID)); // Copiar el PID
    offset += sizeof(pcb->PID);
    memcpy(&pcb->Quantum, buffer + offset, sizeof(pcb->Quantum)); // Copiar el Quantum
    offset += sizeof(pcb->Quantum);
    // Asignar memoria para el Estado
    pcb->State = malloc(strlen(buffer + offset) + 1);
    if (pcb->State == NULL) {
        log_error(logger, "Error al asignar memoria para el Estado: %s", strerror(errno));
        return;
    }
    memcpy(pcb->State, buffer + offset, strlen(buffer + offset) + 1); // Copiar el Estado
    offset += strlen(buffer + offset) + 1;
    memcpy(&pcb->CPU_REGISTERS, buffer + offset, sizeof(pcb->CPU_REGISTERS)); // Copiar los registros de la CPU
}

// Create a packet
t_packet* create_packet(OP_CODES op_code, size_t payload_size, void* data, serializer serializer_func) {
    // Asignar memoria para el paquete
    t_packet* packet = malloc(sizeof(t_packet));
    if (packet == NULL) {
        log_error(logger, "Error al asignar memoria para el paquete: %s", strerror(errno));
        return NULL;
    }

    // Asignar memoria para el payload
    packet->payload = malloc(payload_size);
    if (packet->payload == NULL) {
        log_error(logger, "Error al asignar memoria para el payload: %s", strerror(errno));
        free(packet);
        return NULL;
    }
    // Serializar data
    serializer_func(data, &packet->payload, &payload_size);

    // Asignar los valores del paquete
    packet->op_code = op_code;
    packet->payload_size = payload_size;

    return packet;
}

// Destroy a packet
bool destroy_packet(t_packet* packet) {
    // Liberar la memoria del payload
    free(packet->payload);
    // Liberar la memoria del paquete
    free(packet);
    return true;
}

bool send_packet(int fd, t_packet* packet) {
    // Serializar el paquete
    char* buffer;
    size_t size;
    serialize_packet(packet, &buffer, &size);

    // Enviar el tamaño del paquete
    if (send(fd, &size, sizeof(size_t), 0) < 0) {
        log_error(logger, "Error al enviar el paquete: %s", strerror(errno));
        return false;
    }

    // Enviar el paquete
    if (send(fd, buffer, size, 0) < 0) {
        log_error(logger, "Error al enviar el paquete: %s", strerror(errno));
        return false;
    }

    // Liberar la memoria del buffer
    free(buffer);

    // Los datos se enviaron correctamente
    return true;
}

bool recv_packet(int fd, t_packet* packet) {
    // Recibir el tamaño del paquete
    size_t size;
    if (recv(fd, &size, sizeof(size_t), MSG_WAITALL) < 0) {
        log_error(logger, "Error al recibir el paquete: %s", strerror(errno));
        return false;
    }

    if (size > 1000) {
        return false;
    }

    // Asignar memoria para el buffer
    char* buffer = malloc(size);
    if (buffer == NULL) {
        log_error(logger, "Error al asignar memoria para el buffer: %s", strerror(errno));
        return false;
    }

    // Recibir el paquete
    if (recv(fd, buffer, size, MSG_WAITALL) < 0) {
        log_error(logger, "Error al recibir el paquete: %s", strerror(errno));
        free(buffer);
        return false;
    }

    // Deserializar el paquete
    deserialize_packet(buffer, size, packet);

    // Liberar la memoria del buffer
    free(buffer);

    // Los datos se recibieron correctamente
    return true;
}