//
// Created by utnso on 5/30/24.
//

#include "memoria_management.h"

void init_memory() {
    // Reservar espacio contiguo para la memoria
    int MEMORY_SIZE = memoria_config->tamMemoria;
    int PAGE_SIZE = memoria_config->tamPagina;
    memory = malloc(MEMORY_SIZE);
    if (memory == NULL) {
        perror("Error al reservar memoria");
        exit(EXIT_FAILURE);
    }

    // Inicializar la tabla de páginas
    page_table.num_entries = MEMORY_SIZE / PAGE_SIZE;
    page_table.entries = (t_page_table_entry*)malloc(page_table.num_entries * sizeof(t_page_table_entry));
    if (page_table.entries == NULL) {
        perror("Error al reservar tabla de páginas");
        free(memory);
        exit(EXIT_FAILURE);
    }

    // Marcar todas las entradas como inválidas inicialmente
    for (size_t i = 0; i < page_table.num_entries; i++) {
        page_table.entries[i].valid = 0;
    }
}

void free_memory() {
    free(memory);
    free(page_table.entries);
}

uint32_t translate_address(uint32_t logical_address) {
    int PAGE_SIZE = memoria_config->tamPagina;

    uint32_t page_number = logical_address / PAGE_SIZE;
    uint32_t offset = logical_address % PAGE_SIZE;

    if (page_number >= page_table.num_entries || !page_table.entries[page_number].valid) {
        fprintf(stderr, "Segmentation fault: Invalid page number or page not valid\n");
        exit(EXIT_FAILURE);
    }

    uint32_t frame_number = page_table.entries[page_number].frame_number;
    return frame_number * PAGE_SIZE + offset;
}

void write_memory_from_physical(uint32_t physical_address, uint8_t value) {
    *((uint8_t*)memory + physical_address) = value;
}

void write_memory_from_logical(uint32_t logical_address, uint8_t value) {
    uint32_t physical_address = translate_address(logical_address);
    *((uint8_t*)memory + physical_address) = value;
}

uint8_t read_memory_from_logical(uint32_t logical_address) {
    uint32_t physical_address = translate_address(logical_address);
    return *((uint8_t*)memory + physical_address);
}

void allocate_page(uint32_t page_number, uint32_t frame_number) {
    if (page_number >= page_table.num_entries) {
        fprintf(stderr, "Error: Page number out of range\n");
        return;
    }

    page_table.entries[page_number].frame_number = frame_number;
    page_table.entries[page_number].valid = 1;
}

void assign_page_to_process(t_PCB* pcb) {
    int PAGE_SIZE = memoria_config->tamPagina;

    // Buscar un espacio libre en la memoria para asignarle al proceso
    uint32_t num_pages = pcb->memory_size / PAGE_SIZE;
    uint32_t first_page = 0;
    for (size_t i = 0; i < page_table.num_entries; i++) {
        if (page_table.entries[i].valid) {
            first_page = i + 1;
        } else {
            first_page = 0;
        }

        if (first_page + num_pages >= page_table.num_entries) {
            log_warning(logger, "No hay espacio suficiente para asignarle una página al proceso");
            return;
        }

        if (first_page != 0) {
            break;
        }
    }

    // Asignar las páginas al proceso
    for (size_t i = 0; i < num_pages; i++) {
        allocate_page(first_page + i, first_page + i);
    }
}