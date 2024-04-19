#include "protocol.h"

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
          sizeof(op_code_NUESTRO)   // tamaño del código de operación
        + sizeof(size_t)    // tamaño del total
        + sizeof(size_t)    // tamaño de la cadena
        + size_cadena       // cadena
        + sizeof(uint8_t);  // cantidad

    // Calcula el tamaño de la carga útil (payload)
    size_t size_payload = *size - sizeof(op_code_NUESTRO) - sizeof(size_t);

    // Asigna memoria para el flujo de datos
    void* stream = malloc(*size);

    // Define el código de operación
    op_code_NUESTRO cop = TEST;

    // Copia el código de operación al flujo de datos
    memcpy(stream, &cop, sizeof(op_code_NUESTRO));

    // Copia el tamaño de la carga útil al flujo de datos
    memcpy(stream+sizeof(op_code_NUESTRO), &size_payload, sizeof(size_t));

    // Copia el tamaño de la cadena al flujo de datos
    memcpy(stream+sizeof(op_code_NUESTRO)+sizeof(size_t), &size_cadena, sizeof(size_t));

    // Copia la cadena al flujo de datos
    memcpy(stream+sizeof(op_code_NUESTRO)+sizeof(size_t)*2, cadena, size_cadena);

    // Copia la cantidad al flujo de datos
    memcpy(stream+sizeof(op_code_NUESTRO)+sizeof(size_t)*2+size_cadena, &cant, sizeof(uint8_t));

    // Devuelve el flujo de datos
    return stream;
}

/**
 * @brief Deserializa los datos de prueba desde un flujo de bytes.
 *
 * Esta función toma un flujo de bytes y extrae una cadena y un byte de él.
 * La cadena y el byte extraídos se almacenan en los parámetros cadena y cant, respectivamente.
 *
 * @param stream Un puntero al flujo de bytes a deserializar.
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
    op_code_NUESTRO cop = DEBUG_CODE;

    // Envía el código de depuración a través del socket
    if (send(fd, &cop, sizeof(op_code_NUESTRO), 0) != sizeof(op_code_NUESTRO))
        return false;

    // Devuelve true si el código de depuración se envió correctamente
    return true;
}
