//
// Created by linux-lex on 16/03/24.
//
#include "claves.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main () {
    // Pruebas secuenciales
    // Prueba init: debe devolver 0
    int init_result = init();
    printf("El resultado del init es: %d y el resultado esperado es 0\n", init_result);

    // Pruebas de set_value
    // Prueba 1: almacenar una tupla, debe devolver 0
    int key = 1;
    char value1[20] = "Messi";
    int N_value2 = 5;
    double V_value2[5] = {5.5, 4.5, 3.5, 2.5, 1.5};
    int set_value_id = set_value(key, value1, N_value2, (double *) V_value2);
    printf("El resultado de set value prueba 1 es: %d y el resultado esperado es 0\n", set_value_id);

    // Prueba 2: N-value2 muy alto, debe devolver -1
    key = 2;
    char value1_2[20] = "Messi";
    N_value2 = 33;
    double V_value2_2[5] = {5.5, 4.5, 3.5, 2.5, 1.5};
    set_value_id = set_value(key, value1_2, N_value2, (double *) V_value2_2);
    printf("El resultado de set value prueba 2 es: %d y el resultado esperado es -1\n", set_value_id);

    // Prueba 3: cadena value1 demasiado larga, debe devolver -1
    key = 3;
    char value1_3[258];
    memset(value1_3, 'A', 257);
    value1_3[257] = '\0';
    N_value2 = 5;
    double V_value2_3[5] = {5.5, 4.5, 3.5, 2.5, 1.5};
    set_value_id = set_value(key, value1_3, N_value2, (double *) V_value2_3);
    printf("El resultado de set value prueba 3 es: %d y el resultado esperado es -1\n", set_value_id);

    // Prueba 4: Hacer set sobre tupla existente, debe devolver -1
    key = 1;
    set_value_id = set_value(key, value1, N_value2, (double *) V_value2);
    printf("El resultado de set value prueba 4 es: %d y el resultado esperado es -1\n", set_value_id);

    // Pruebas de get_value
    // Prueba 1: recuperar un valor existente, debe devolver 0
    char value1_4[255];
    double V_value2_4[32];
    int get_value_id = get_value(key, value1_4, &N_value2, V_value2_4);
    printf("El resultado de get value prueba 1 es: %d y el resultado esperado es 0\n", get_value_id);
    printf("Get value result: %s, %d, %f\n", value1_4, N_value2, V_value2_4[0]);

    // Prueba 2: recuperar valor inexistente, debe devolver -1
    key = 2;
    char value1_5[255];
    double V_value2_5[32];
    get_value_id = get_value(key, value1_5, &N_value2, V_value2_5);
    printf("El resultado de get value prueba 2 es: %d y el resultado esperado es -1\n", get_value_id);

    // Pruebas de modify_value
    // Prueba 1: modificar un valor, debe devolver 0
    key = 1;
    char value1_6[20] = "CR7";
    N_value2 = 3;
    double V_value2_6[3] = {3.5, 2.5, 1.5};
    int modify_value_id = modify_value(key, value1_6, N_value2, (double *) V_value2_6);
    printf("El resultado de modify value prueba 1 es: %d y el resultado esperado es 0\n", modify_value_id);

    char value1_7[255];
    double V_value2_7[32];
    get_value(key, value1_7, &N_value2, V_value2_7);
    printf("Get value result: %s, %d, %f\n", value1_7, N_value2, V_value2_7[0]);

    // Prueba 2: modificar un valor inexistente, debe devolver -1
    key = 2;
    modify_value_id = modify_value(key, value1_6, N_value2, (double *) V_value2_6);
    printf("El resultado de modify value prueba 2 es: %d y el resultado esperado es -1\n", modify_value_id);

    // Prueba 3: n_value muy alto, debe devolver -1
    key = 1;
    N_value2 = 35;
    modify_value_id = modify_value(key, value1_6, N_value2, (double *) V_value2_6);
    printf("El resultado de modify value prueba 3 es: %d y el resultado esperado es -1\n", modify_value_id);

    // Prueba 4: cadena muy larga
    N_value2 = 3;
    modify_value_id = modify_value(key, value1_3, N_value2, (double *) V_value2_6);
    printf("El resultado de modify value prueba 4 es: %d y el resultado esperado es -1\n", modify_value_id);

    // Pruebas de exist
    // Prueba 1: mirar un elemento existente, debe devolver 1
    int exist_id = exist(key);
    printf("El resultado de exist prueba 1 es: %d y el resultado esperado es 1\n", exist_id);

    // Prueba 2: mirar un elemento inexistente, debe devolver 0
    key = 2;
    exist_id = exist(key);
    printf("El resultado de exist prueba 2 es: %d y el resultado esperado es 0\n", exist_id);

    // Pruebas de delete_key
    // Prueba 1: borrar un elemento existente, debe devolver 0
    key = 1;
    int delete_value_id = delete_key(key);
    printf("El resultado de delete_key prueba 1 es: %d y el resultado esperado es 0\n", delete_value_id);

    char value1_8[255];
    double V_value2_8[32];
    int get_id = get_value(key, value1_8, &N_value2, V_value2_8);
    printf("Get value result: %d\n", get_id);

    // Prueba 2: borrar un elemento no existente, debe devolver -1
    delete_value_id = delete_key(key);
    printf("El resultado de delete_key prueba 2 es: %d y el resultado esperado es -1\n", delete_value_id);
    return 0;
}