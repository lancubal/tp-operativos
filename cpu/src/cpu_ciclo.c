//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"


void set_register_value(const char* reg_name, uint32_t value) {
    if (strcmp(reg_name, "PC") == 0) CPU_Registers.PC = value;
    else if (strcmp(reg_name, "AX") == 0) CPU_Registers.AX = (uint8_t)value;
    else if (strcmp(reg_name, "BX") == 0) CPU_Registers.BX = (uint8_t)value;
    else if (strcmp(reg_name, "CX") == 0) CPU_Registers.CX = (uint8_t)value;
    else if (strcmp(reg_name, "DX") == 0) CPU_Registers.DX = (uint8_t)value;
    else if (strcmp(reg_name, "EAX") == 0) CPU_Registers.EAX = value;
    else if (strcmp(reg_name, "EBX") == 0) CPU_Registers.EBX = value;
    else if (strcmp(reg_name, "EDX") == 0) CPU_Registers.EDX = value;
    else if (strcmp(reg_name, "ECX") == 0) CPU_Registers.ECX = value;
    else if (strcmp(reg_name, "SI") == 0) CPU_Registers.SI = value;
    else if (strcmp(reg_name, "DI") == 0) CPU_Registers.DI = value;
    else if (strcmp(reg_name, "size") == 0) CPU_Registers.size = value;
    else log_warning(logger, "Registro desconocido: %s", reg_name);
}

uint32_t get_register_value(const char* reg_name) {
    if (strcmp(reg_name, "PC") == 0) return CPU_Registers.PC;
    else if (strcmp(reg_name, "AX") == 0) return CPU_Registers.AX;
    else if (strcmp(reg_name, "BX") == 0) return CPU_Registers.BX;
    else if (strcmp(reg_name, "CX") == 0) return CPU_Registers.CX;
    else if (strcmp(reg_name, "DX") == 0) return CPU_Registers.DX;
    else if (strcmp(reg_name, "EAX") == 0) return CPU_Registers.EAX;
    else if (strcmp(reg_name, "EBX") == 0) return CPU_Registers.EBX;
    else if (strcmp(reg_name, "EDX") == 0) return CPU_Registers.EDX;
    else if (strcmp(reg_name, "ECX") == 0) return CPU_Registers.ECX;
    else if (strcmp(reg_name, "SI") == 0) return CPU_Registers.SI;
    else if (strcmp(reg_name, "DI") == 0) return CPU_Registers.DI;
    else if (strcmp(reg_name, "size") == 0) return CPU_Registers.size;
    else {
        log_warning(logger, "Registro desconocido: %s", reg_name);
        return 0;
    }
}
uint32_t read_memory(uint32_t address) {
    // TODO: Implementación para leer de memoria
}

void write_memory(uint32_t address, uint32_t data) {
    // TODO: Implementación para escribir en memoria
}

bool resize_memory(uint32_t new_size) {
    // TODO: Implementación para redimensionar la memoria
}

void handle_out_of_memory() {
    // TODO: Implementación para manejar el caso de memoria insuficiente
}

void copy_memory(uint32_t src, uint32_t dest, uint32_t size) {
    // TODO: Implementación para copiar memoria
}

void request_resource(const char* resource) {
    // TODO: Implementación para solicitar un recurso
}

void release_resource(const char* resource) {
    // TODO: Implementación para liberar un recurso
}

void io_gen_sleep(uint32_t interface, uint32_t units) {
    // TODO: Implementación para realizar una operación de sleep en I/O
}

void io_stdin_read(uint32_t interface, uint32_t address, uint32_t size) {
    // TODO: Implementación para leer desde STDIN
}

void io_stdout_write(uint32_t interface, uint32_t address, uint32_t size) {
    // TODO: Implementación para escribir en STDOUT
}

void io_fs_create(uint32_t interface, const char* filename) {
    // TODO: Implementación para crear un archivo en el sistema de archivos
}

void io_fs_delete(uint32_t interface, const char* filename) {
    // TODO: Implementación para eliminar un archivo en el sistema de archivos
}

void io_fs_truncate(uint32_t interface, const char* filename, uint32_t size) {
    // TODO: Implementación para truncar un archivo en el sistema de archivos
}

void io_fs_write(uint32_t interface, const char* filename, uint32_t address, uint32_t size, uint32_t file_pointer) {
    // TODO: Implementación para escribir en un archivo en el sistema de archivos
}

void io_fs_read(uint32_t interface, const char* filename, uint32_t address, uint32_t size, uint32_t file_pointer) {
    // TODO: Implementación para leer de un archivo en el sistema de archivos
}

void handle_exit() {
    // TODO: Implementación para manejar la finalización del proceso
}


void fetch() {
    log_warning(logger, "Fetching...");
    // Enviar el PC con el opcode FETCH
    send_packet(sockets->memoriaSocket, create_packet(FETCH, sizeof(CPU_Registers.PC), &CPU_Registers.PC, NULL));
    // Semaforo para esperar la instruccion de memoria
    sem_wait(&sem_instruccion);
    log_warning(logger, "Instruccion recibida: %s", instruccion);
    CPU_Registers.PC++;
}
/*Esta etapa consiste en interpretar qué instrucción es la que se va a ejecutar
 y si la misma requiere de una traducción de dirección lógica a dirección física. */
void decode() {
    decoded_instruction = malloc(sizeof(instruction_decoded_t));
    char *token = strtok(instruccion, " ");
    if (token == NULL) return;

    if (strcmp(token, "SET") == 0) {
        decoded_instruction->type = SET;
    } else if (strcmp(token, "SUM") == 0) {
        decoded_instruction->type = SUM;
    } else if (strcmp(token, "SUB") == 0) {
        decoded_instruction->type = SUB;
    } else if(strcmp(token, "MOV") == 0) {
        decoded_instruction->type = MOV;
    } else if (strcmp(token, "MOV_IN") == 0) {
        decoded_instruction->type = MOV_IN;
    } else if (strcmp(token, "MOV_OUT") == 0) {
        decoded_instruction->type = MOV_OUT;
    } else if (strcmp(token, "RESIZE") == 0) {
        decoded_instruction->type = RESIZE;
    } else if (strcmp(token, "JNZ") == 0) {
        decoded_instruction->type = JNZ;
    } else if (strcmp(token, "COPY_STRING") == 0) {
        decoded_instruction->type = COPY_STRING;
    } else if (strcmp(token, "IO_GEN_SLEEP") == 0) {
        decoded_instruction->type = IO_GEN_SLEEP;
    } else if (strcmp(token, "IO_STDIN_READ") == 0) {
        decoded_instruction->type = IO_STDIN_READ;
    } else if (strcmp(token, "IO_STDOUT_WRITE") == 0) {
        decoded_instruction->type = IO_STDOUT_WRITE;
    } else if (strcmp(token, "IO_FS_CREATE") == 0) {
        decoded_instruction->type = IO_FS_CREATE;
    } else if (strcmp(token, "IO_FS_DELETE") == 0) {
        decoded_instruction->type = IO_FS_DELETE;
    } else if (strcmp(token, "IO_FS_TRUNCATE") == 0) {
        decoded_instruction->type = IO_FS_TRUNCATE;
    } else if (strcmp(token, "IO_FS_WRITE") == 0) {
        decoded_instruction->type = IO_FS_WRITE;
    } else if (strcmp(token, "IO_FS_READ") == 0) {
        decoded_instruction->type = IO_FS_READ;
    } else if (strcmp(token, "WAIT") == 0) {
        decoded_instruction->type = WAIT;
    } else if (strcmp(token, "SIGNAL") == 0) {
        decoded_instruction->type = SIGNAL;
    } else if (strcmp(token, "EXIT") == 0) {
        decoded_instruction->type = EXIT;
    } else {
        // Instrucción desconocida
        log_warning(logger, "Instrucción desconocida: %s", token);
        return;
    }

    // Obtener los operandos
    int operand_index = 0;
    while ((token = strtok(NULL, " ")) != NULL && operand_index < 5) {
        strncpy(decoded_instruction->operands[operand_index], token, sizeof(decoded_instruction->operands[operand_index]) - 1);
        decoded_instruction->operands[operand_index][sizeof(decoded_instruction->operands[operand_index]) - 1] = '\0';
        operand_index++;
    }
    // Iterar mostrando todos los valores
    log_warning(logger, "Instrucción: %d", decoded_instruction->type);
    for (int i = 0; i < operand_index; i++) {
        log_warning(logger, "Operando: %s", decoded_instruction->operands[i]);
    }

}

void execute() {
    if (decoded_instruction == NULL) {
        log_warning(logger, "No hay instrucción para ejecutar");
        return;
    }
    char *endptr;
    switch (decoded_instruction->type) {
        case SET: {
            // SET (Registro, Valor)
            const char* reg_name = decoded_instruction->operands[0];
            uint32_t  value = strtol(decoded_instruction->operands[1], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para SET: %s", decoded_instruction->operands[1]);
                break;
            }
            set_register_value(reg_name, value);
            break;
        }
        case MOV_IN: {
            // MOV_IN (Registro Datos, Registro Dirección)
            const char* reg_data = decoded_instruction->operands[0];
            const char* reg_dir = decoded_instruction->operands[1];
            uint32_t address = get_register_value(reg_dir);
            uint32_t data = read_memory(address);
            set_register_value(reg_data, data);
            break;
        }
        case MOV_OUT: {
            // MOV_OUT (Registro Dirección, Registro Datos)
            const char* reg_dir = decoded_instruction->operands[0];
            const char* reg_data = decoded_instruction->operands[1];
            uint32_t address = get_register_value(reg_dir);
            uint32_t data = get_register_value(reg_data);
            write_memory(address, data);
            break;
        }
        case SUM: {
            // SUM (Registro Destino, Registro Origen)
            const char* reg_dest = decoded_instruction->operands[0];
            const char* reg_src = decoded_instruction->operands[1];
            uint32_t dest_value = get_register_value(reg_dest);
            uint32_t src_value = get_register_value(reg_src);
            set_register_value(reg_dest, dest_value + src_value);
            break;
        }
        case SUB: {
            // SUB (Registro Destino, Registro Origen)
            const char* reg_dest = decoded_instruction->operands[0];
            const char* reg_src = decoded_instruction->operands[1];
            uint32_t dest_value = get_register_value(reg_dest);
            uint32_t src_value = get_register_value(reg_src);
            set_register_value(reg_dest, dest_value - src_value);
            break;
        }
        case JNZ: {
            // JNZ (Registro, Instrucción)
            const char* reg = decoded_instruction->operands[0];
            uint32_t instruction_number = strtol(decoded_instruction->operands[1], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para JNZ: %s", decoded_instruction->operands[1]);
                break;
            }
            uint32_t reg_value = get_register_value(reg);
            if (reg_value != 0) {
                CPU_Registers.PC = instruction_number;
            }
            break;
        }
        case RESIZE: {
            // RESIZE (Tamaño)
            uint32_t new_size = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para RESIZE: %s", decoded_instruction->operands[0]);
                break;
            }
            if (!resize_memory(new_size)) {
                handle_out_of_memory();
            }
            break;
        }
        case COPY_STRING: {
            // COPY_STRING (Tamaño)
            uint32_t size = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para COPY_STRING: %s", decoded_instruction->operands[0]);
                break;
            }
            uint32_t src_address = get_register_value("SI");
            uint32_t dest_address = get_register_value("DI");
            copy_memory(src_address, dest_address, size);
            break;
        }
        case WAIT: {
            // WAIT (Recurso)
            const char* resource = decoded_instruction->operands[0];
            request_resource(resource);
            break;
        }
        case SIGNAL: {
            // SIGNAL (Recurso)
            const char* resource = decoded_instruction->operands[0];
            release_resource(resource);
            break;
        }
        case IO_GEN_SLEEP: {
            // IO_GEN_SLEEP (Interfaz, Unidades de trabajo)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_GEN_SLEEP: %s", decoded_instruction->operands[0]);
                break;
            }
            uint32_t units = strtol(decoded_instruction->operands[1], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_GEN_SLEEP: %s", decoded_instruction->operands[1]);
                break;
            }
            io_gen_sleep(interface, units);
            break;
        }
        case IO_STDIN_READ: {
            // IO_STDIN_READ (Interfaz, Registro Dirección, Registro Tamaño)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_STDIN_READ: %s", decoded_instruction->operands[0]);
                break;
            }
            uint32_t address = get_register_value(decoded_instruction->operands[1]);
            uint32_t size = get_register_value(decoded_instruction->operands[2]);
            io_stdin_read(interface, address, size);
            break;
        }
        case IO_STDOUT_WRITE: {
            // IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_STDOUT_WRITE: %s", decoded_instruction->operands[0]);
                break;
            }
            uint32_t address = get_register_value(decoded_instruction->operands[1]);
            uint32_t size = get_register_value(decoded_instruction->operands[2]);
            io_stdout_write(interface, address, size);
            break;
        }
        case IO_FS_CREATE: {
            // IO_FS_CREATE (Interfaz, Nombre Archivo)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_FS_CREATE: %s", decoded_instruction->operands[0]);
                break;
            }
            const char* filename = decoded_instruction->operands[1];
            io_fs_create(interface, filename);
            break;
        }
        case IO_FS_DELETE: {
            // IO_FS_DELETE (Interfaz, Nombre Archivo)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_FS_DELETE: %s", decoded_instruction->operands[0]);
                break;
            }
            const char* filename = decoded_instruction->operands[1];
            io_fs_delete(interface, filename);
            break;
        }
        case IO_FS_TRUNCATE: {
            // IO_FS_TRUNCATE (Interfaz, Nombre Archivo, Registro Tamaño)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_FS_TRUNCATE: %s", decoded_instruction->operands[0]);
                break;
            }
            const char* filename = decoded_instruction->operands[1];
            uint32_t size = get_register_value(decoded_instruction->operands[2]);
            io_fs_truncate(interface, filename, size);
            break;
        }
        case IO_FS_WRITE: {
            // IO_FS_WRITE (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_FS_WRITE: %s", decoded_instruction->operands[0]);
                break;
            }
            const char* filename = decoded_instruction->operands[1];
            uint32_t address = get_register_value(decoded_instruction->operands[2]);
            uint32_t size = get_register_value(decoded_instruction->operands[3]);
            uint32_t file_pointer = get_register_value(decoded_instruction->operands[4]);
            io_fs_write(interface, filename, address, size, file_pointer);
            break;
        }
        case IO_FS_READ: {
            // IO_FS_READ (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
            uint32_t interface = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para IO_FS_READ: %s", decoded_instruction->operands[0]);
                break;
            }
            const char* filename = decoded_instruction->operands[1];
            uint32_t address = get_register_value(decoded_instruction->operands[2]);
            uint32_t size = get_register_value(decoded_instruction->operands[3]);
            uint32_t file_pointer = get_register_value(decoded_instruction->operands[4]);
            io_fs_read(interface, filename, address, size, file_pointer);
            break;
        }
        case EXIT: {
            // EXIT
            handle_exit();
            break;
        }
        default:
            log_warning(logger, "Instrucción no soportada: %d", decoded_instruction->type);
            break;
    }

}

void cpu_ciclo() {
    // TODO: Implementar el ciclo de la CPU
    while(true) {
        sem_wait(&sem_pcb);
        int i = 0;
        while (true) {
            fetch();
            decode();
            execute();
            i++;
            if (i > 3) {
                sem_post(&sem_cycle);
                break;
            }
        }
    }
}