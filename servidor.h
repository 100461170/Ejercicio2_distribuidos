//
// Created by linux-lex on 29/02/24.
//
#include "structures.h"

#ifndef EJERCICIO1_DISTRIBUIDOS_SERVIDOR_H
#define EJERCICIO1_DISTRIBUIDOS_SERVIDOR_H

void * tratar_peticion(void *p);
int s_init();
int s_set_value(int key, char *valor1, int valor2_N, double *valor2_value);
int s_get_value(int key, char *valor1, int * valor2_N, double *valor2_value);
int s_modify_value(int key, char *valor1, int valor2_N, double *valor2_value);
int s_delete_key(int key);
int s_exist(int key);
int load();
int write_back();
void close_server();

#endif //EJERCICIO1_DISTRIBUIDOS_SERVIDOR_H
