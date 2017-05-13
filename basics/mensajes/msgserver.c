/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server.c
 * Author: iker
 *
 * Created on 12 de mayo de 2017, 13:51
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//mensajes
#include <sys/ipc.h>
#include <sys/msg.h>

#define SIZE 256
#define CLAVE 0x55555555L

typedef struct msg_cola {
    long id;
    char datos[SIZE];
} msg_cola;

int main(int argc, char** argv) {

    int id_cola;
    int readBytes, err;
    msg_cola msg;
    printf("Proceso servidor, creando la cola de mensajes:\n");

    id_cola = msgget(CLAVE, 0666 | IPC_CREAT);

    if (id_cola == -1) {
        printf("Error creando la cola de mensajes. Saliendo...\n");
        exit(0);
    }

    printf("Cola creada. Procediendo a su lectura en 30 segundos... \n");

    sleep(30);

    err = msgrcv(id_cola, (struct msgbuf*) &msg, sizeof (msg.id) + sizeof (msg.datos), 1200, 0);

    if (err == -1) {
        printf("Error leyendo de la cola de mensajes. Saliendo...\n");
        exit(0);
    }

    printf("Se ha leido correctamente el siguiente mensaje:\n- %s", msg.datos);
    //fflush(stdout);

    printf("Destruyendo la cola y saliendo...\n-");

    err = msgctl(id_cola, IPC_RMID, (struct msqid_ds*) NULL);

    if (err == -1) {
        printf("Error destruyendo la cola de mensajes. Saliendo...\n");
        exit(0);
    }


    return (EXIT_SUCCESS);
}

