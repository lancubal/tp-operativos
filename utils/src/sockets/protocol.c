#include "protocol.h"

// Test
static void* serializar_test(size_t* size, char* cadena, uint8_t cant) {
    size_t size_cadena = strlen(cadena) + 1;
    *size =
          sizeof(op_code_NUESTRO)   // cop
        + sizeof(size_t)    // total
        + sizeof(size_t)    // size de char* cadena
        + size_cadena         // char* cadena
        + sizeof(uint8_t);  // cant
    size_t size_payload = *size - sizeof(op_code_NUESTRO) - sizeof(size_t);

    void* stream = malloc(*size);

    op_code_NUESTRO cop = TEST;
    memcpy(stream, &cop, sizeof(op_code_NUESTRO));
    memcpy(stream+sizeof(op_code_NUESTRO), &size_payload, sizeof(size_t));
    memcpy(stream+sizeof(op_code_NUESTRO)+sizeof(size_t), &size_cadena, sizeof(size_t));
    memcpy(stream+sizeof(op_code_NUESTRO)+sizeof(size_t)*2, cadena, size_cadena);
    memcpy(stream+sizeof(op_code_NUESTRO)+sizeof(size_t)*2+size_cadena, &cant, sizeof(uint8_t));

    return stream;
}

static void deserializar_test(void* stream, char** cadena, uint8_t* cant) {
    // cadena
    size_t size_cadena;
    memcpy(&size_cadena, stream, sizeof(size_t));

    char* r_cadena = malloc(size_cadena);
    memcpy(r_cadena, stream+sizeof(size_t), size_cadena);
    *cadena = r_cadena;

    // Pochoclos
    memcpy(cant, stream+sizeof(size_t)+size_cadena, sizeof(uint8_t));
}

bool send_test(int fd, char* cadena, uint8_t cant) {
    size_t size;
    void* stream = serializar_test(&size, cadena, cant);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_test(int fd, char** cadena, uint8_t* cant) {
    size_t size_payload;
    if (recv(fd, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
        return false;

    void* stream = malloc(size_payload);
    if (recv(fd, stream, size_payload, 0) != size_payload) {
        free(stream);
        return false;
    }

    deserializar_test(stream, cadena, cant);

    free(stream);
    return true;
}

// DEBUG
bool send_debug(int fd) {
    op_code_NUESTRO cop = DEBUG_CODE;
    if (send(fd, &cop, sizeof(op_code_NUESTRO), 0) != sizeof(op_code_NUESTRO))
        return false;
    return true;
}
