//
// Created by utnso on 4/14/24.
//

#include "cpu_conexion.h"



/**
 * @brief Inicia las conexiones necesarias para el funcionamiento de la CPU.
 *
 * Esta función se encarga de iniciar las conexiones con los servidores de despacho (Dispatch),
 * interrupciones (Interrupt) y memoria. Utiliza la configuración de la CPU para obtener las direcciones IP
 * y los puertos necesarios para estas conexiones.
 *
 * @param cpuConfig La configuración de la CPU.
 */
void iniciarConexiones(cpu_config_t* cpuConfig) {
    sockets = malloc(sizeof(socketsT));
    // Iniciamos el servidor de despacho (Dispatch) utilizando la IP y el puerto especificados en la configuración de la CPU.
    sockets->dispatchSocket = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaDispatch, "CPU Dispatch");

    // Iniciamos el servidor de interrupciones (Interrupt) utilizando la IP y el puerto especificados en la configuración de la CPU.
    sockets->interruptSocket = iniciarServerProceso(cpuConfig->ipCPU, cpuConfig->puertoEscuchaInterrupt, "CPU Interrupt");

    // Nos conectamos al servidor de memoria utilizando la IP y el puerto especificados en la configuración de la CPU.
    sockets->memoriaSocket = connectToServer(cpuConfig->ipMemoria,cpuConfig->puertoMemoria);
}


/**
 * @brief Procesa una conexión entrante.
 *
 * Esta función se encarga de procesar una conexión entrante. Recibe un puntero a una estructura que contiene
 * el descriptor de archivo del socket y el nombre del servidor. Luego entra en un bucle donde recibe y procesa
 * mensajes hasta que el cliente se desconecta.
 *
 * @param void_args Un puntero a una estructura t_procesar_conexion_args.
 */
void procesar_conexion(t_procesar_conexion_args* conexion_args) {

    // Obtenemos el descriptor de archivo del socket y el nombre del servidor
    int cliente_socket = conexion_args->fd;
    char* server_name = conexion_args->server_name;

    // Liberamos la memoria de la estructura de argumentos
    free(conexion_args);

    // Entramos en un bucle donde recibimos y procesamos mensajes
    while (cliente_socket != -1) {
        // Recibir un paquete del cliente
        t_packet* packet = malloc(sizeof(t_packet));
        if (recv_packet(cliente_socket, packet)) {
            log_info(logger, "Recibido mensaje con código de operación: %d\n", packet->op_code);
        }

        switch (packet->op_code) {
            case PCB: {
                // Deserializo el PCB
                t_PCB *pcb = malloc(sizeof(t_PCB));
                deserialize_pcb(packet->payload, packet->payload_size, pcb);
                if(pcb != NULL) {
                    // Cargo el contexto de ejecución y logeo los datos del PCB
                    load_context(&CPU_Registers, &pcb->CPU_REGISTERS);
                    log_info(logger, "Recibido PCB con PID: %d\n", pcb->PID);
                    log_info(logger, "Recibido PCB con Estado: %s\n", pcb->State);
                    log_info(logger, "Recibido PCB con PC: %d\n", pcb->CPU_REGISTERS.PC);
                    log_info(logger, "CPU PC %d", CPU_Registers.PC);

                    // Semaforo para avisarle a cpu_ciclo que ya se recibio el pcb
                    sem_post(&sem_pcb);

                    // Enviar PCB a CPU
                    sem_wait(&sem_cycle);
                    log_warning(logger, "Fetch terminado.");
                    send_packet(cliente_socket, create_packet(PCB, pcb->size, pcb, serialize_pcb));
                }
                free(pcb);
                break;
            }
            case FETCH: {
                // Recibo la instruccion
                instruccion = malloc(packet->payload_size);
                memcpy(instruccion, packet->payload, packet->payload_size);
                sem_post(&sem_instruccion);
            }
            default: {
                break;
            }
        }
        destroy_packet(packet);
    }

    // Si el cliente se desconectó, registramos una advertencia
    log_warning(logger, "El cliente se desconecto de %s server", server_name);
}

/**
 * @brief Escucha conexiones entrantes en un socket de servidor.
 *
 * Esta función espera una conexión entrante en el socket de servidor especificado.
 * Cuando se establece una conexión, crea un nuevo hilo para procesarla y luego vuelve a esperar más conexiones.
 *
 * @param server_name El nombre del servidor.
 * @param server_socket Un puntero al descriptor de archivo del socket del servidor.
 * @return 1 si se pudo establecer una conexión, 0 en caso contrario.
 */
int server_escuchar(char* server_name, const int* server_socket) {
    // Esperamos una conexión entrante en el socket del servidor
    int cliente_socket = esperar_cliente(server_name, *server_socket);

    // Si se estableció una conexión
    if (cliente_socket != -1) {
        // Creamos un nuevo hilo para procesar la conexión
        pthread_t hilo;
        t_procesar_conexion_args* conexion_args = malloc(sizeof(t_procesar_conexion_args));
        conexion_args->fd = cliente_socket;
        conexion_args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) conexion_args);
        pthread_detach(hilo);
        return 1;
    }
    // Si no se pudo establecer una conexión, retornamos 0
    return 0;
}

// Escuchar al servidor conectado
void cliente_escuchar(int* client_socket) {
    // debo usar procesar_conexion y correrlo.
    pthread_t hilo;
    t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
    args->fd = *client_socket;
    args->server_name = "test";
    pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
    pthread_detach(hilo);
}

/**
 * @brief Escucha conexiones entrantes en un socket de servidor en un bucle.
 *
 * Esta función entra en un bucle donde llama a server_escuchar para esperar y procesar conexiones entrantes.
 * El bucle continúa hasta que server_escuchar retorna 0, lo que indica que no se pudo establecer una conexión.
 *
 * @param server_socket Un puntero al descriptor de archivo del socket del servidor.
 * @return -1 cuando se sale del bucle.
 */
int phread_server_escuchar(void* server_socket){
    // Entramos en un bucle donde esperamos y procesamos conexiones entrantes
    while (server_escuchar("CPU",(int*)server_socket));
    return -1;
}

/**
 * @brief Finaliza todas las conexiones y libera los recursos utilizados.
 *
 * Esta función se encarga de desconectar todos los sockets de servidor y cliente,
 * destruir la estructura de sockets y liberar los recursos utilizados por el logger.
 *
 */
void fin_conexion(){
    // Desconectamos los sockets de servidor
    disconnectServer(sockets->dispatchSocket);
    disconnectServer(sockets->interruptSocket);
    // Desconectamos el socket de cliente
    disconnectClient(sockets->memoriaSocket);
    // Registramos un mensaje indicando que el servidor CPU ha terminado
    log_info(logger,"Terminado el Servidor CPU");
    // Liberamos los recursos utilizados por el logger
    log_destroy(logger);
}