//
// Created by linux-lex on 29/02/24.
//
#include "claves.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char * argv[])
{
    srand(time(NULL) * getpid()); // Inicialización adecuada de la semilla
    // Generación aleatoria de la operación y la clave
    int random_operation = rand() % 6;
    int random_key;
    if (argc > 1 && strcmp(argv[1], "1") == 0) {
        random_key = 1;
    }
    else {
        random_key = (rand() % 5) + 1; // Clave en el rango de 1 a 10
    }
    switch (random_operation)
    {
    case 0: // Operación 'init'
        printf("Cliente con pid %d, operación 'init'\n", getpid());
        int init_result = init();
        printf("Cliente con pid %d, operación 'init' realizada, el resultado es %d\n", getpid(), init_result);
        break;
    case 1: // Operación 'set'
    {
        double values[3] = {1.5, 2.5, 3.5};
        printf("Cliente con pid %d, operación 'set_value' para la clave %d, la tupla a insertar es {value, 3, [%f, %f, %f]}\n",
               getpid(), random_key, values[0], values[1], values[2]);
        int set_value_result = set_value(random_key, "value", 3, (double *) values);
        printf("Cliente con pid %d, operación 'set_value' para la clave %d realizada, el resultado es %d\n", getpid(),
               random_key, set_value_result);
    }
        break;
    case 2: // Operación 'get'
        printf("Cliente con pid %d, operación 'get_value' para la clave %d\n", getpid(), random_key);
        char value1[255];
        int N_value2;
        double V_value2[32];
        int get_value_result = get_value(random_key, value1, &N_value2, V_value2);
        printf("Cliente con pid %d, operación 'get_value' para la clave %d realizada, el resultado es %d\n", getpid(), random_key, get_value_result);
        if (get_value_result == 0) {
            printf("Cliente con pid %d, la tupla devuelta para la clave %d es {%s, %d, [%f, %f, %f]}\n", getpid(), random_key, value1, N_value2, V_value2[0], V_value2[1], V_value2[2]);
        }
        break;
    case 3: //operacion modify
    {
        double values_m[3] = {4.5, 5.5, 6.5};
        printf("Cliente con pid %d, operación 'modify_value' para la clave es %d, la tupla por la que se va a sustituir es {value2, 3, [%f, %f, %f]}\n",
               getpid(), random_key, values_m[0], values_m[1], values_m[2]);
        int modify_value_result = modify_value(random_key, "value_m", 3, (double *) values_m);
        printf("Cliente con pid %d, operación 'modify_value' para la clave %d realizada, el resultado es %d\n",
               getpid(), random_key, modify_value_result);
    }
        break;
    case 4: // Operación 'delete'
        printf("Cliente con pid %d, operación 'delete_key' para la clave %d\n", getpid(), random_key);
        int delete_key_result = delete_key(random_key);
        printf("Cliente con pid %d, operación 'delete_key' para la clave %d realizada, el resultado es %d\n", getpid(), random_key, delete_key_result);
        break;
    case 5: // Operación 'exist'
        printf("Cliente con pid %d, operación 'exist' para la clave %d\n", getpid(), random_key);
        int exist_result = exist(random_key);
        printf("Cliente con pid %d, operación 'exist' para la clave %d realizada, el resultado es %d\n", getpid(), random_key, exist_result);
        break;
    }
    return 0;
}
