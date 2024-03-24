//
// Created by linux-lex on 1/03/24.
//

#ifndef EJERCICIO1_DISTRIBUIDOS_STRUCTURES_H
#define EJERCICIO1_DISTRIBUIDOS_STRUCTURES_H

#define MAX 256
#define MAX_VECTOR 32

struct peticion {
    int op;
    int key;
    char valor1[MAX];
    int valor2_N;
    int* valor2_N_p;
    double valor2_value[MAX_VECTOR];
    char q_name[MAX];
};

struct respuesta {
    int status;
    char valor1[MAX];
    int N_value2;
    double valor2_value[MAX_VECTOR];
};

struct tupla {
    int clave;
    char valor1[MAX];
    int valor2_N;
    double valor2_value[MAX_VECTOR];
};


#endif //EJERCICIO1_DISTRIBUIDOS_STRUCTURES_H