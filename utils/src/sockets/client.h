//
// Created by utnso on 4/5/24.
//

#ifndef TP_2024_1C_GRUPO_CLIENT_H
#define TP_2024_1C_GRUPO_CLIENT_H

#include<unistd.h>
#include<netdb.h>
#include <pthread.h>
#include <errno.h>
#include <utils/logger.h>
#include "sockets/conexiontad.h"

int connectToServer(char*, char*);
void disconnectClient(int);

#endif //TP_2024_1C_GRUPO_CLIENT_H
