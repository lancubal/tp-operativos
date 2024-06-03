//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"


void set_register_value(const char* reg_name, uint32_t value) {
    if (strcmp(reg_name, "PC") == 0) pcb->CPU_REGISTERS.PC = value;
    else if (strcmp(reg_name, "AX") == 0) pcb->CPU_REGISTERS.AX = (uint8_t)value;
    else if (strcmp(reg_name, "BX") == 0) pcb->CPU_REGISTERS.BX = (uint8_t)value;
    else if (strcmp(reg_name, "CX") == 0) pcb->CPU_REGISTERS.CX = (uint8_t)value;
    else if (strcmp(reg_name, "DX") == 0) pcb->CPU_REGISTERS.DX = (uint8_t)value;
    else if (strcmp(reg_name, "EAX") == 0) pcb->CPU_REGISTERS.EAX = value;
    else if (strcmp(reg_name, "EBX") == 0) pcb->CPU_REGISTERS.EBX = value;
    else if (strcmp(reg_name, "EDX") == 0) pcb->CPU_REGISTERS.EDX = value;
    else if (strcmp(reg_name, "ECX") == 0) pcb->CPU_REGISTERS.ECX = value;
    else if (strcmp(reg_name, "SI") == 0) pcb->CPU_REGISTERS.SI = value;
    else if (strcmp(reg_name, "DI") == 0) pcb->CPU_REGISTERS.DI = value;
    else log_warning(logger, "Registro desconocido: %s", reg_name);
}

uint32_t get_register_value(const char* reg_name) {
    if (strcmp(reg_name, "PC") == 0) return pcb->CPU_REGISTERS.PC;
    else if (strcmp(reg_name, "AX") == 0) return pcb->CPU_REGISTERS.AX;
    else if (strcmp(reg_name, "BX") == 0) return pcb->CPU_REGISTERS.BX;
    else if (strcmp(reg_name, "CX") == 0) return pcb->CPU_REGISTERS.CX;
    else if (strcmp(reg_name, "DX") == 0) return pcb->CPU_REGISTERS.DX;
    else if (strcmp(reg_name, "EAX") == 0) return pcb->CPU_REGISTERS.EAX;
    else if (strcmp(reg_name, "EBX") == 0) return pcb->CPU_REGISTERS.EBX;
    else if (strcmp(reg_name, "EDX") == 0) return pcb->CPU_REGISTERS.EDX;
    else if (strcmp(reg_name, "ECX") == 0) return pcb->CPU_REGISTERS.ECX;
    else if (strcmp(reg_name, "SI") == 0) return pcb->CPU_REGISTERS.SI;
    else if (strcmp(reg_name, "DI") == 0) return pcb->CPU_REGISTERS.DI;
    else {
        log_warning(logger, "Registro desconocido: %s", reg_name);
        return 0;
    }
}


void fetch() {
    log_warning(logger, "Fetching...");
    // Enviar el PC con el opcode FETCH
    send_packet(sockets->memoriaSocket, create_packet(FETCH, sizeof(pcb->CPU_REGISTERS.PC), &pcb->CPU_REGISTERS.PC, NULL));
    // Semaforo para esperar la instruccion de memoria
    sem_wait(&sem_instruccion);
    log_warning(logger, "Instruccion recibida: %s", instruccion);
    pcb->CPU_REGISTERS.PC++;
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
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_STDIN_READ") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_STDOUT_WRITE") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_FS_CREATE") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_FS_DELETE") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_FS_TRUNCATE") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_FS_WRITE") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "IO_FS_READ") == 0) {
        decoded_instruction->type = IO;
    } else if (strcmp(token, "WAIT") == 0) {
        decoded_instruction->type = I_WAIT;
    } else if (strcmp(token, "SIGNAL") == 0) {
        decoded_instruction->type = I_SIGNAL;
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
            // TODO: MOV_IN (Registro Datos, Registro Dirección)
            const char* reg_data = decoded_instruction->operands[0];
            const char* reg_dir = decoded_instruction->operands[1];
            uint32_t address = get_register_value(reg_dir);
            //uint32_t data = read_memory(address);
            //set_register_value(reg_data, data);
            break;
        }
        case MOV_OUT: {
            // TODO: MOV_OUT (Registro Dirección, Registro Datos)
            const char* reg_dir = decoded_instruction->operands[0];
            const char* reg_data = decoded_instruction->operands[1];
            uint32_t address = get_register_value(reg_dir);
            uint32_t data = get_register_value(reg_data);
            //write_memory(address, data);
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
                pcb->CPU_REGISTERS.PC = instruction_number;
            }
            break;
        }
        case RESIZE: {
            // TODO: RESIZE (Tamaño)
            uint32_t new_size = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para RESIZE: %s", decoded_instruction->operands[0]);
                break;
            }
            /*if (!resize_memory(new_size)) {
                handle_out_of_memory();
            }*/
            break;
        }
        case COPY_STRING: {
            // TODO: COPY_STRING (Tamaño)
            uint32_t size = strtol(decoded_instruction->operands[0], &endptr, 10);
            if (*endptr != '\0') {
                log_warning(logger, "Valor inválido para COPY_STRING: %s", decoded_instruction->operands[0]);
                break;
            }
            uint32_t src_address = get_register_value("SI");
            uint32_t dest_address = get_register_value("DI");
            //copy_memory(src_address, dest_address, size);
            break;
        }
        case I_WAIT: {
            // TODO: WAIT (Recurso)
            const char* resource = decoded_instruction->operands[0];
            // TODO
            //request_resource(resource);
            break;
        }
        case I_SIGNAL: {
            // TODO: SIGNAL (Recurso)
            const char* resource = decoded_instruction->operands[0];
            //release_resource(resource);
            break;
        }
        case IO: {
            // Desalojar proceso
            // Mandar el opcode WAIT con la interface y el tiempo al kernel
            strcpy(pcb->recurso[0], decoded_instruction->operands[0]);
            for (int i = 1; i < 5; i++) {
                if (decoded_instruction->operands[i] == NULL) break;
                strcpy(pcb->recurso[i], decoded_instruction->operands[i]);
            }

            send_packet(sockets->interruptSocket, create_packet(WAIT, sizeof(WAIT), pcb, serialize_pcb));
            // terminar
            interrupcion = true;
            break;
        }
        case EXIT: {
            // EXIT
            interrupcion = true;
            break;
        }
        default:
            // No hay más instrucciones
            log_warning(logger, "El proceso ejecuto todas las instrucciones");
            interrupcion = true;
            break;
    }

}

void cpu_ciclo() {
    while(true) {
        sem_wait(&sem_pcb);
        while (true) {
            fetch();
            decode();
            execute();
            if (interrupcion) {
                interrupcion = false;
                sem_post(&sem_cycle);
                break;
            }
        }
    }
}