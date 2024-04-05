//
// Created by utnso on 4/5/24.
//

#ifndef TP_2024_1C_GRUPO_LOGGER_H
#define TP_2024_1C_GRUPO_LOGGER_H

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<readline/readline.h>

void readFromConsole(t_log*);
t_log* loggerCreate(void);

#endif //TP_2024_1C_GRUPO_LOGGER_H

