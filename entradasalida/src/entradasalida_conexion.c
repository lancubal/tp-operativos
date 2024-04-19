//
// Created by utnso on 4/14/24.
//

#include "entradasalida_conexion.h"

/**
 * @brief Inicia las conexiones con los servidores de Memoria y Kernel.
 *
 * Esta función se encarga de iniciar las conexiones con los servidores de Memoria y Kernel.
 * Utiliza la configuración de Entrada/Salida para obtener las direcciones IP y los puertos necesarios para estas conexiones.
 *
 * @param entradasalidaConfig La configuración de Entrada/Salida.
 */
void iniciarConexiones(entradasalida_config_t * entradasalidaConfig){
    // Nos conectamos al servidor de Memoria utilizando la IP y el puerto especificados en la configuración de Entrada/Salida.
    sockets.memoriaSocket = connectToServer(entradasalidaConfig->ipMemoria,entradasalidaConfig->puertoMemoria);
    // Nos conectamos al servidor de Kernel utilizando la IP y el puerto especificados en la configuración de Entrada/Salida.
    sockets.kernelSocket = connectToServer(entradasalidaConfig->ipKernel,entradasalidaConfig->puertoKernel);
}

/**
 * @brief Finaliza todas las conexiones y libera los recursos utilizados.
 *
 * Esta función se encarga de desconectar todos los sockets de cliente,
 * y liberar los recursos utilizados por el logger.
 *
 */
void fin_conexion(){
    // Desconectamos el socket de cliente de Memoria
    disconnectClient(sockets.memoriaSocket);
    // Desconectamos el socket de cliente de Kernel
    disconnectClient(sockets.kernelSocket);
    // Registramos un mensaje indicando que el servidor de Entrada/Salida ha terminado
    log_info(logger,"Terminado I/O");
    // Liberamos los recursos utilizados por el logger
    log_destroy(logger);
}