
/* 
 * File:   client.c
 * Author: iker
 *
 * Created on 12 de mayo de 2017, 10:34
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
    char buffer [SIZE];
    msg_cola msg;

    printf("Proceso cliente, escriba un mensaje para enviar al servidor:\n");
    fgets(buffer, SIZE, stdin);
    printf("Mensaje leido: %s \n", buffer);

    id_cola = msgget(CLAVE, 0666);

    if (id_cola == -1) {
        printf("Error abriendo la cola de mensajes. Saliendo...\n");
        exit(0);
    }

    msg.id = 1200;
    strcpy(msg.datos, buffer);
    err = msgsnd(id_cola, (struct msgbuf*) &msg, strlen(msg.datos), 0);

    if (err == -1) {
        printf("Error escribiendo en la cola de mensajes. Saliendo...\n");
        exit(0);
    }
    
    sleep(30);


    return (EXIT_SUCCESS);
}

