//
// Created by linux-lex on 29/02/24.
//

#include "claves.h"
#include <unistd.h>
#include "structures.h"
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>


int init(){
    struct peticion p;
    struct respuesta r;
    mqd_t queue_servidor;
    mqd_t queue_cliente;
    char client_name[MAX];
    // abrir colas
    open_client(&queue_cliente, client_name);
    if (-1 == queue_cliente){
        perror("Error en cliente. Mq_open queue cliente");
        return -1;
    }
    open_server(&queue_servidor);
    if (-1 == queue_servidor){
        close_client(&queue_cliente, client_name);
        perror("Error en cliente. Mq_open queue servidor");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 0;
    strcpy(p.q_name, client_name);
    // mandar peticion al servidor
    int send_p = send_server(&queue_servidor, (const char *)&p, sizeof(struct peticion), 0);
    if (-1 == send_p){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_send");
        return -1;
    }
    // recibir respuesta
    int recv_r = receive_client(&queue_cliente, (char *)&r, sizeof(struct respuesta), 0);
    if (-1 == recv_r){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_recv");
        return -1;
    }
    close_queues(&queue_servidor, &queue_cliente, client_name);
    return r.status;

}

int set_value(int key, char *value1, int N_value2, double *V_value2){
    struct peticion p;
    struct respuesta r;
    mqd_t queue_servidor;
    mqd_t queue_cliente;
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
    // abrir colas
    open_client(&queue_cliente, client_name);
    if (-1 == queue_cliente){
        perror("Error en cliente. Mq_open queue cliente");
        return -1;
    }
    open_server(&queue_servidor);
    if (-1 == queue_servidor){
        close_client(&queue_cliente, client_name);
        perror("Error en cliente. Mq_open queue servidor");
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
    int send_p = send_server(&queue_servidor, (const char *)&p, sizeof(struct peticion), 0);
    if (-1 == send_p){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_send");
        return -1;
    }
    // recibir respuesta
    int recv_r = receive_client(&queue_cliente, (char *)&r, sizeof(struct respuesta), 0);
    if (-1 == recv_r){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_recv");
        return -1;
    }
    close_queues(&queue_servidor, &queue_cliente, client_name);
    return r.status;


}

int get_value(int key, char *value1, int *N_value2, double *V_value2){
    struct peticion p;
    struct respuesta r;
    mqd_t queue_servidor;
    mqd_t queue_cliente;
    char client_name[MAX];
    // abrir colas
    open_client(&queue_cliente, client_name);
    if (-1 == queue_cliente){
        perror("Error en cliente. Mq_open queue cliente");
        return -1;
    }
    open_server(&queue_servidor);
    if (-1 == queue_servidor){
        close_client(&queue_cliente, client_name);
        perror("Error en cliente. Mq_open queue servidor");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 2;
    strcpy(p.q_name, client_name);
    p.key = key;

    // mandar peticion al servidor
    int send_p = send_server(&queue_servidor, (const char *)&p, sizeof(struct peticion), 0);
    if (-1 == send_p){
        perror("Error en cliente. Mq_send");
        return -1;
    }
    // recibir respuesta
    int recv_r = receive_client(&queue_cliente, (char *)&r, sizeof(struct respuesta), 0);
    if (-1 == recv_r){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_recv");
        return -1;
    }
    // copiar valores
    strcpy(value1, r.valor1);
    *N_value2 = r.N_value2;
    for (int i = 0; i<r.N_value2; i++){
        V_value2[i] = r.valor2_value[i];
    }
    close_queues(&queue_servidor, &queue_cliente, client_name);
    return r.status;

}

int modify_value(int key, char *value1, int N_value2, double *V_value2){
    struct peticion p;
    struct respuesta r;
    mqd_t queue_servidor;
    mqd_t queue_cliente;
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
    // abrir colas
    open_client(&queue_cliente, client_name);
    if (-1 == queue_cliente){
        perror("Error en cliente. Mq_open queue cliente");
        return -1;
    }
    open_server(&queue_servidor);
    if (-1 == queue_servidor){
        close_client(&queue_cliente, client_name);
        perror("Error en cliente. Mq_open queue servidor");
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
    int send_p = send_server(&queue_servidor, (const char *)&p, sizeof(struct peticion), 0);
    if (-1 == send_p){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_send");
        return -1;
    }
    // recibir respuesta
    int recv_r = receive_client(&queue_cliente, (char *)&r, sizeof(struct respuesta), 0);
    if (-1 == recv_r){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_recv");
        return -1;
    }
    close_queues(&queue_servidor, &queue_cliente, client_name);
    return r.status;

}
int delete_key(int key){
    struct peticion p;
    struct respuesta r;
    mqd_t queue_servidor;
    mqd_t queue_cliente;
    char client_name[MAX];
    // abrir colas
    open_client(&queue_cliente, client_name);
    if (-1 == queue_cliente){
        perror("Error en cliente. Mq_open queue cliente");
        return -1;
    }
    open_server(&queue_servidor);
    if (-1 == queue_servidor){
        close_client(&queue_cliente, client_name);
        perror("Error en cliente. Mq_open queue servidor");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 4;
    strcpy(p.q_name, client_name);
    p.key = key;
    // mandar peticion al servidor
    int send_p = send_server(&queue_servidor, (const char *)&p, sizeof(struct peticion), 0);
    if (-1 == send_p){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_send");
        return -1;
    }
    // recibir respuesta
    int recv_r = receive_client(&queue_cliente, (char *)&r, sizeof(struct respuesta), 0);
    if (-1 == recv_r){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_recv");
        return -1;
    }
    close_queues(&queue_servidor, &queue_cliente, client_name);
    return r.status;
}

int exist(int key){
    struct peticion p;
    struct respuesta r;
    mqd_t queue_servidor;
    mqd_t queue_cliente;
    char client_name[MAX];
    // abrir colas
    open_client(&queue_cliente, client_name);
    if (-1 == queue_cliente){
        perror("Error en cliente. Mq_open queue cliente");
        return -1;
    }
    open_server(&queue_servidor);
    if (-1 == queue_servidor){
        close_client(&queue_cliente, client_name);
        perror("Error en cliente. Mq_open queue servidor");
        return -1;
    }
    // Rellenar la peticion
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    memset(&p, 0, sizeof(struct peticion));
    p.op = 5;
    strcpy(p.q_name, client_name);
    p.key = key;
    // mandar peticion al servidor
    int send_p = send_server(&queue_servidor, (const char *)&p, sizeof(struct peticion), 0);
    if (-1 == send_p){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_send");
        return -1;
    }
    // recibir respuesta
    int recv_r = receive_client(&queue_cliente, (char *)&r, sizeof(struct respuesta), 0);
    if (-1 == recv_r){
        close_queues(&queue_servidor, &queue_cliente, client_name);
        perror("Error en cliente. Mq_recv");
        return -1;
    }
    close_queues(&queue_servidor, &queue_cliente, client_name);
    return r.status;

}

void open_client(mqd_t *queue_cliente, char *client_name){
    sprintf(client_name, "%s%d", "/CLIENTE_", getpid());
    // abrir cola cliente
    struct mq_attr attr_cliente;
    attr_cliente.mq_maxmsg = 10;
    attr_cliente.mq_msgsize = sizeof(struct respuesta);
    *queue_cliente = mq_open(client_name, O_CREAT | O_RDONLY, 0700, &attr_cliente);
}

void open_server(mqd_t * queue_servidor){
    // crear nombre cola
    const static char queue_name_s[1024] = "/SERVIDOR";
    // abrir cola servidor
    struct mq_attr attr_servidor;
    attr_servidor.mq_maxmsg = 10;
    attr_servidor.mq_msgsize = sizeof(struct peticion);
    *queue_servidor = mq_open(queue_name_s, O_CREAT| O_WRONLY, 0700, &attr_servidor);
}

int send_server(mqd_t * queue_servidor,const char * message, int size, unsigned int prio){
    int send = mq_send(*queue_servidor, message, size, prio);
    return send;
}

int receive_client(mqd_t *queue_cliente, char *message, int size, unsigned int *prio){
    int receive = mq_receive(*queue_cliente, message, size, prio);
    return receive;
}

void close_queues(mqd_t *queue_servidor, mqd_t *queue_cliente, char *client_name){
    mq_close(*queue_servidor);
    mq_close(*queue_cliente);
    mq_unlink(client_name);
}

void close_client(mqd_t *queue_cliente, char *client_name) {
    mq_close(*queue_cliente);
    mq_unlink(client_name);
}

