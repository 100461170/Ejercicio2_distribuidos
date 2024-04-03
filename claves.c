//
// Created by linux-lex on 29/02/24.
//

#include "claves.h"
#include <unistd.h>
#include "communications.h"
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>


int init(){
    struct peticion p;
    struct respuesta r;
    char client_name[MAX];
    int sd, ret;
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }

    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 0;
    strcpy(p.q_name, client_name);
    // mandar peticion al servidor
    ret = sendMessage(sd, (char *) &p, sizeof(struct peticion));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, (char *) &r, sizeof(struct respuesta));
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    return r.status;

}

int set_value(int key, char *value1, int N_value2, double *V_value2){
    struct peticion p;
    struct respuesta r;
    char client_name[MAX];
    int sd, ret;
    // comprobar errores
    if (N_value2 < 1 || N_value2 > 32){
        fprintf(stderr, "Error: N_value2 no esta en el rango [1,32].\n");
        return -1;
    }
    if (strlen(value1)>MAX){
        fprintf(stderr, "Error: la cadena valor1 tiene mas de 256 caracteres.\n");
        return -1;
    }
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0){
        perror("Error en clientSocket");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());

    memset(&p, 0, sizeof(struct peticion));
    p.op = 1;
    strcpy(p.q_name, client_name);
    p.key = key;
    strcpy(p.valor1, value1);
    p.valor2_N = N_value2;
    // copiar vector
    for (int i = 0; i < N_value2; i++) {
        p.valor2_value[i] = V_value2[i];
    }
    // mandar peticion al servidor
    ret = sendMessage(sd, (char *) &p, sizeof(struct peticion));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, (char *) &r, sizeof(struct respuesta));
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    return r.status;


}

int get_value(int key, char *value1, int *N_value2, double *V_value2){
    struct peticion p;
    struct respuesta r;
    char client_name[MAX];
    int sd, ret;
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 2;
    strcpy(p.q_name, client_name);
    p.key = key;

    // mandar peticion al servidor
    ret = sendMessage(sd, (char *) &p, sizeof(struct peticion));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, (char *) &r, sizeof(struct respuesta));
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    // copiar valores
    strcpy(value1, r.valor1);
    *N_value2 = r.N_value2;
    for (int i = 0; i<r.N_value2; i++){
        V_value2[i] = r.valor2_value[i];
    }
    closeSocket(sd);
    return r.status;

}

int modify_value(int key, char *value1, int N_value2, double *V_value2){
    struct peticion p;
    struct respuesta r;
    int sd, ret;
    char client_name[MAX];
    // comprobar errores
    if (N_value2 < 1 || N_value2 > 32){
        fprintf(stderr, "Error: N_value2 no esta en el rango [1,32].\n");
        return -1;
    }
    if (strlen(value1)>MAX){
        fprintf(stderr, "Error: la cadena valor1 tiene mas de 256 caracteres.\n");
        return -1;
    }
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 3;
    strcpy(p.q_name, client_name);
    p.key = key;
    strcpy(p.valor1, value1);
    for (int i = 0; i < N_value2; i++){
        p.valor2_value[i] = V_value2[i];
    }
    p.valor2_N = N_value2;

    // mandar peticion al servidor
    ret = sendMessage(sd, (char *)&p, sizeof(struct peticion));
    if (ret == -1){
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, (char *)&r, sizeof(struct respuesta));
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    return r.status;

}
int delete_key(int key){
    struct peticion p;
    struct respuesta r;
    int sd, ret;
    char client_name[MAX];
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 4;
    strcpy(p.q_name, client_name);
    p.key = key;
    // mandar peticion al servidor
    ret = sendMessage(sd, (char *)&p, sizeof(struct peticion));
    if (ret == -1){
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, (char *)&r, sizeof(struct respuesta));
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    return r.status;
}

int exist(int key){
    struct peticion p;
    struct respuesta r;
    int sd, ret;
    char client_name[MAX];
    // abrir socket
    sd = clientSocket("localhost", 4200);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 5;
    strcpy(p.q_name, client_name);
    p.key = key;
    // mandar peticion al servidor
    ret = sendMessage(sd, (char *)&p, sizeof(struct peticion));
    if (ret == -1){
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, (char *)&r, sizeof(struct respuesta));
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    return r.status;

}




