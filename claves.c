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
    char resp[MAX];
    int sd, ret;
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    if (ip_tuplas == NULL || port_tuplas == NULL){
        fprintf(stderr, "Variables de entorno no definidas.\n");
        return -1;
    }
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }

    // mandar numero operacion
    ret = sendMessage(sd, "0", sizeof(char));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, resp, sizeof(char)*MAX);
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    int retorno = strtol(resp, &ending_char, 10);
    return retorno;
}

int set_value(int key, char *value1, int N_value2, double *V_value2){
    char resp[MAX];
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
    if (ip_tuplas == NULL || port_tuplas == NULL){
        fprintf(stderr, "Variables de entorno no definidas.\n");
        return -1;
    }
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // mandar numero operacion
    ret = sendMessage(sd, "1", sizeof(char));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar key
    char c_key[MAX];
    sprintf(c_key, "%d", key);
    ret = sendMessage(sd, c_key, sizeof(c_key));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar value1
    ret = sendMessage(sd, value1, sizeof(value1));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar N_value2
    char c_N_value2[MAX];
    sprintf(c_N_value2, "%d", N_value2);
    ret = sendMessage(sd, c_N_value2, sizeof(N_value2));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }

    // mandar V_value2
    for (int i = 0; i< N_value2; i++){
        char vector_string[MAX];
        snprintf(vector_string, MAX, "%f", V_value2[i]);
        ret = sendMessage(sd, vector_string, sizeof(vector_string));
        if (ret == -1){
            perror("Error en envio");
            return -1;
        }
    }
    
    // recibir respuesta
    ret = recvMessage(sd, resp, sizeof(char)*MAX);
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    int retorno = strtol(resp, &ending_char, 10);
    return retorno;

}

int get_value(int key, char *value1, int *N_value2, double *V_value2){
    // struct peticion p;
    // struct respuesta r;    
    char resp[MAX];
    int sd, ret;
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    if (ip_tuplas == NULL || port_tuplas == NULL){
        fprintf(stderr, "Variables de entorno no definidas.\n");
        return -1;
    }
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // mandar peticion al servidor
    // mandar numero operacion    
    ret = sendMessage(sd, "2", sizeof(char));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar key
    char c_key[MAX];
    sprintf(c_key, "%d", key);
    ret = sendMessage(sd, c_key, sizeof(c_key));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }    
    // recibir respuesta
    ret = recvMessage(sd, resp, sizeof(char)*MAX);
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    // si respuesta es negativa terminar
    int retorno = strtol(resp, &ending_char, 10);
    if (retorno == -1){
        closeSocket(sd);
        return retorno;
    }    
    // conseguir elementos
    char c_valor2_N[MAX];
    char c_valor2_value[MAX_VECTOR];   
    // recibir value1
    ret = recvMessage(sd, value1, sizeof(char) * MAX);
    if (ret == -1) {
        perror("Error en recibo");
        return -1;
    }
    
    // recibir N_value2
    ret = recvMessage(sd, c_valor2_N, sizeof(char)*MAX);
    if (ret == -1) {
        perror("Error en recibo");
        return -1;
    }
    // convertir tipos
    // N_value2
    *N_value2 = strtol(c_valor2_N, &ending_char, 10);
    // recibir V_value2
    for (int i = 0; i < *N_value2; i++){
        ret = recvMessage(sd, c_valor2_value, sizeof(char) * MAX);
        if (ret == -1){
            perror("Error en recibo");
            return -1;
        }
        V_value2[i] = strtold(c_valor2_value, &ending_char);
    }
    closeSocket(sd);
    return retorno;

}

int modify_value(int key, char *value1, int N_value2, double *V_value2){
    char resp[MAX];
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
    if (ip_tuplas == NULL || port_tuplas == NULL){
        fprintf(stderr, "Variables de entorno no definidas.\n");
        return -1;
    }
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // mandar numero operacion
    ret = sendMessage(sd, "3", sizeof(char));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar key
    char c_key[MAX];
    sprintf(c_key, "%d", key);
    ret = sendMessage(sd, c_key, sizeof(c_key));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar value1
    ret = sendMessage(sd, value1, sizeof(value1));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar N_value2
    char c_N_value2[MAX];
    sprintf(c_N_value2, "%d", N_value2);
    ret = sendMessage(sd, c_N_value2, sizeof(c_N_value2));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }

    // mandar V_value2
    for (int i = 0; i< N_value2; i++){
        char vector_string[MAX];
        snprintf(vector_string, MAX, "%f", V_value2[i]);
        ret = sendMessage(sd, vector_string, sizeof(vector_string));
        if (ret == -1)
        {
            perror("Error en envio");
            return -1;
        }
    }
    // recibir respuesta
    ret = recvMessage(sd, resp, sizeof(char)*MAX);
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    int retorno = strtol(resp, &ending_char, 10);
    return retorno;

}
int delete_key(int key){
    int sd, ret;
    char resp[MAX];
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    if (ip_tuplas == NULL || port_tuplas == NULL){
        fprintf(stderr, "Variables de entorno no definidas.\n");
        return -1;
    }
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // // mandar peticion al servidor
    // mandar numero operacion
    ret = sendMessage(sd, "4", sizeof(char));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar key
    char c_key[MAX];
    sprintf(c_key, "%d", key);
    ret = sendMessage(sd, c_key, sizeof(c_key));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, resp, sizeof(char)*MAX);
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    int retorno = strtol(resp, &ending_char, 10);
    return retorno;
}

int exist(int key){
    int sd, ret;
    char resp[MAX];
    // abrir socket
    // abrir socket
    char *ip_tuplas, *port_tuplas, *ending_char;
    ip_tuplas = getenv("IP_TUPLAS");
    port_tuplas = getenv("PORT_TUPLAS");
    if (ip_tuplas == NULL || port_tuplas == NULL){
        fprintf(stderr, "Variables de entorno no definidas.\n");
        return -1;
    }
    int port_number = strtol(port_tuplas, &ending_char, 10);
    sd = clientSocket(ip_tuplas, port_number);
    if (sd < 0) {
        perror("Error en clientSocket");
        return -1;
    }
    // mandar numero operacion
    ret = sendMessage(sd, "5", sizeof(char));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // mandar key
    char c_key[MAX];
    sprintf(c_key, "%d", key);
    ret = sendMessage(sd, c_key, sizeof(c_key));
    if (ret == -1) {
        perror("Error en envio");
        return -1;
    }
    // recibir respuesta
    ret = recvMessage(sd, resp, sizeof(char)*MAX);
    if (ret == -1){
        perror("Error en recepcion");
        return -1;
    }
    closeSocket(sd);
    int retorno = strtol(resp, &ending_char, 10);
    return retorno;

}




