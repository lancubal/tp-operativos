//
// Created by utnso on 5/30/24.
//

#ifndef TP_2024_V2_ENTRADASALIDA_INTERFACES_H
#define TP_2024_V2_ENTRADASALIDA_INTERFACES_H

#include "entradasalida_config.h"

typedef enum {
    GENERIC,
    STDIN,
    STRDOUT,
    DIALFS
} interface_type ;

typedef struct {
    interface_type type;
    char* name;
} t_interface;

void start_interface();

#endif //TP_2024_V2_ENTRADASALIDA_INTERFACES_H
