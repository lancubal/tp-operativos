//
// Created by utnso on 4/14/24.
//

#include "cpu_ciclo.h"

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
    while ((token = strtok(NULL, " ")) != NULL && operand_index < 3) {
        strncpy(decoded_instruction->operands[operand_index], token, sizeof(decoded_instruction->operands[operand_index]) - 1);
        decoded_instruction->operands[operand_index][sizeof(decoded_instruction->operands[operand_index]) - 1] = '\0';
        operand_index++;
    }

    log_warning(logger, "Instrucción decodificada: %d", decoded_instruction->type);
}

void execute() {
    // TODO: Ejecutar la instruccion
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