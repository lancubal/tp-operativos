#include <commons/log.h>
#include <errno.h>
#include "protocol.h"

bool send_opcode(int fd, OP_CODES opcode) {
    // Enviar el opcode
    ssize_t bytes_sent = send(fd, &opcode, sizeof(OP_CODES), 0);
    if (bytes_sent < 0) {
        log_error(logger, "Error al enviar el opcode: %s", strerror(errno));
        return false;
    }

    // Verificar si se envió el tamaño correcto
    if ((size_t)bytes_sent != sizeof(OP_CODES)) {
        log_error(logger, "Error al enviar el opcode: tamaño incorrecto");
        return false;
    }

    // El opcode se envió correctamente
    return true;
}

bool recv_opcode(int fd, OP_CODES* opcode) {
    // Recibir el opcode
    /*ssize_t bytes_received = recv(fd, opcode, sizeof(OP_CODES), 0);
    if (bytes_received < 0) {
        log_error(logger, "Error al recibir el opcode: %s", strerror(errno));
        return false;
    }

    // Verificar si se recibió el tamaño correcto
    if ((size_t)bytes_received != sizeof(OP_CODES)) {
        log_error(logger, "Error al recibir el opcode: tamaño incorrecto");
        return false;
    }*/

    // El opcode se recibió correctamente
    if(recv(fd, opcode, sizeof(OP_CODES), 0)) {
        return true;
    }
    return false;
}

void* serializer(void* tad, const size_t *size) {
    // Asigna memoria para el flujo de bytes
    void* stream = malloc(*size);

    // Copia la estructura al flujo de bytes
    memcpy(stream, tad, *size);

    return stream;
}

void* deserializer(void* stream, const size_t *size) {
    // Asigna memoria para la estructura
    void* tad = malloc(*size);

    // Copia el flujo de bytes a la estructura
    memcpy(tad, stream, *size);

    return tad;
}

bool send_tad(int fd, void* tad, const size_t *size) {
    // Serializa la estructura en un flujo de bytes
    void* stream = serializer(tad, size);

    // Envía la longitud de los datos del TAD como prefijo
    if (send(fd, size, sizeof(size_t), 0) < 0) {
        // Si el envío no se realiza correctamente, libera la memoria y devuelve false
        free(stream);
        log_error(logger, "Error al enviar la longitud de los datos del TAD: %s", strerror(errno));
        return false;
    }

    // Envía el flujo de bytes a través del socket
    if (send(fd, stream, *size, 0) != *size) {
        // Si el envío no se realiza correctamente, libera la memoria y devuelve false
        free(stream);
        return false;
    }

    // Libera la memoria asignada para el flujo de bytes
    free(stream);

    // Devuelve true si los datos se enviaron correctamente
    return true;
}

bool recv_tad(int fd, void** tad) {

    // Recibir la longitud de los datos del TAD como prefijo
    size_t tad_size;
    if (recv(fd, &tad_size, sizeof(size_t), 0) < 0) {
        log_error(logger, "Error al recibir la longitud de los datos del TAD: %s", strerror(errno));
        return false;
    }

    // Asignar memoria para los datos del TAD
    *tad = malloc(tad_size);
    if (*tad == NULL) {
        log_error(logger, "Error al asignar memoria para los datos del TAD: %s", strerror(errno));
        return false;
    }

    // Recibir los datos reales del TAD
    if (recv(fd, *tad, tad_size, 0) < 0) {
        log_error(logger, "Error al recibir los datos del TAD: %s", strerror(errno));
        return false;
    }

    // Los datos se recibieron correctamente
    return true;
}

// Test
/**
 * @brief Serializa los datos de prueba en un flujo de bytes.
 *
 * Esta función toma una cadena y un byte, calcula el tamaño de los datos serializados,
 * asigna la memoria necesaria y luego copia los datos en la memoria asignada.
 * El tamaño de los datos serializados se almacena en el parámetro size.
 *
 * @param size Un puntero a un size_t donde se almacenará el tamaño de los datos serializados.
 * @param cadena La cadena a ser serializada.
 * @param cant El byte a ser serializado.
 * @return Un puntero a los datos serializados.
 */
static void* serializar_test(size_t* size, char* cadena, uint8_t cant) {
    // Calcula el tamaño de la cadena más el caracter nulo al final
    size_t size_cadena = strlen(cadena) + 1;

    // Calcula el tamaño total de los datos serializados
    *size =
          sizeof(OP_CODES)   // tamaño del código de operación
        + sizeof(size_t)    // tamaño del total
        + sizeof(size_t)    // tamaño de la cadena
        + size_cadena       // cadena
        + sizeof(uint8_t);  // cantidad

    // Calcula el tamaño de la carga útil (payload)
    size_t size_payload = *size - sizeof(OP_CODES) - sizeof(size_t);

    // Asigna memoria para el flujo de datos
    void* stream = malloc(*size);

    // Define el código de operación
    OP_CODES cop = TEST;

    // Copia el código de operación al flujo de datos
    memcpy(stream, &cop, sizeof(OP_CODES));

    // Copia el tamaño de la carga útil al flujo de datos
    memcpy(stream+sizeof(OP_CODES), &size_payload, sizeof(size_t));

    // Copia el tamaño de la cadena al flujo de datos
    memcpy(stream+sizeof(OP_CODES)+sizeof(size_t), &size_cadena, sizeof(size_t));

    // Copia la cadena al flujo de datos
    memcpy(stream+sizeof(OP_CODES)+sizeof(size_t)*2, cadena, size_cadena);

    // Copia la cantidad al flujo de datos
    memcpy(stream+sizeof(OP_CODES)+sizeof(size_t)*2+size_cadena, &cant, sizeof(uint8_t));

    // Devuelve el flujo de datos
    return stream;
}

/**
 * @brief Deserializa los datos de prueba desde un flujo de bytes.
 *
 * Esta función toma un flujo de bytes y extrae una cadena y un byte de él.
 * La cadena y el byte extraídos se almacenan en los parámetros cadena y cant, respectivamente.
 *
 * @param stream Un puntero al flujo de bytes a deserializer.
 * @param cadena Un puntero a un puntero de char donde se almacenará la cadena deserializada.
 * @param cant Un puntero a un uint8_t donde se almacenará el byte deserializado.
 */
static void deserializar_test(void* stream, char** cadena, uint8_t* cant) {
    // Extrae el tamaño de la cadena del flujo de bytes
    size_t size_cadena;
    memcpy(&size_cadena, stream, sizeof(size_t));

    // Asigna memoria para la cadena y la extrae del flujo de bytes
    char* r_cadena = malloc(size_cadena);
    memcpy(r_cadena, stream+sizeof(size_t), size_cadena);
    *cadena = r_cadena;

    // Extrae el byte del flujo de bytes
    memcpy(cant, stream+sizeof(size_t)+size_cadena, sizeof(uint8_t));
}

/**
 * @brief Envía datos de prueba a través de un socket.
 *
 * Esta función serializa una cadena y un byte en un flujo de bytes y luego lo envía a través de un socket.
 * Si el envío no se realiza correctamente, la función libera la memoria asignada para el flujo de bytes y devuelve false.
 *
 * @param fd El descriptor de archivo del socket a través del cual se enviarán los datos.
 * @param cadena La cadena a ser enviada.
 * @param cant El byte a ser enviado.
 * @return true si los datos se enviaron correctamente, false en caso contrario.
 */
bool send_test(int fd, char* cadena, uint8_t cant) {
    // Serializa los datos de prueba en un flujo de bytes
    size_t size;
    void* stream = serializar_test(&size, cadena, cant);

    // Envía el flujo de bytes a través del socket
    if (send(fd, stream, size, 0) != size) {
        // Si el envío no se realiza correctamente, libera la memoria y devuelve false
        free(stream);
        return false;
    }

    // Libera la memoria asignada para el flujo de bytes
    free(stream);

    // Devuelve true si los datos se enviaron correctamente
    return true;
}

/**
 * @brief Recibe datos de prueba a través de un socket.
 *
 * Esta función recibe un flujo de bytes a través de un socket y luego deserializa una cadena y un byte de él.
 * Si la recepción no se realiza correctamente, la función libera la memoria asignada para el flujo de bytes y devuelve false.
 *
 * @param fd El descriptor de archivo del socket a través del cual se recibirán los datos.
 * @param cadena Un puntero a un puntero de char donde se almacenará la cadena recibida.
 * @param cant Un puntero a un uint8_t donde se almacenará el byte recibido.
 * @return true si los datos se recibieron correctamente, false en caso contrario.
 */
bool recv_test(int fd, char** cadena, uint8_t* cant) {
    // Recibe el tamaño de la carga útil
    size_t size_payload;
    if (recv(fd, &size_payload, sizeof(size_t), 0) != sizeof(size_t))
        return false;

    // Asigna memoria para el flujo de bytes
    void* stream = malloc(size_payload);

    // Recibe el flujo de bytes a través del socket
    if (recv(fd, stream, size_payload, 0) != size_payload) {
        // Si la recepción no se realiza correctamente, libera la memoria y devuelve false
        free(stream);
        return false;
    }

    // Deserializa los datos de prueba desde el flujo de bytes
    deserializar_test(stream, cadena, cant);

    // Libera la memoria asignada para el flujo de bytes
    free(stream);

    // Devuelve true si los datos se recibieron correctamente
    return true;
}

/**
 * @brief Envía un código de depuración a través de un socket.
 *
 * Esta función envía un código de depuración a través de un socket.
 * Si el envío no se realiza correctamente, la función devuelve false.
 *
 * @param fd El descriptor de archivo del socket a través del cual se enviará el código de depuración.
 * @return true si el código de depuración se envió correctamente, false en caso contrario.
 */
bool send_debug(int fd) {
    // Define el código de depuración
    OP_CODES cop = DEBUG_CODE;

    // Envía el código de depuración a través del socket
    if (send(fd, &cop, sizeof(OP_CODES), 0) != sizeof(OP_CODES))
        return false;

    // Devuelve true si el código de depuración se envió correctamente
    return true;
}
