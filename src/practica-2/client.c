/* 
 * File:   client.c
 * Author: iker
 *
 * Created on 29 de marzo de 2017, 8:27
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define SIZE 256
#define PATH1 "/tmp/fifo_monitor_1"
#define PATH2 "/tmp/fifo_monitor_2"
#define CLAVE 0x72477396L

typedef struct msg_cola {
    long id;
    char data[SIZE];
} msg_cola;

void checkError(int err, int type);

int main(int argc, char** argv) {

    int fifo1, fifo2;
    int readbytes, err;
    int id_cola;
    char buffer[SIZE];
    char input[SIZE];
    msg_cola msg;

    if (argc != 2) {
        printf("Error, número de argumentos de entrada incorrecto. Saliendo del programa...\n");
        exit(0);
    }

    printf("Pulsa una tecla para comenzar el ejercicio 1.\n");
    fgets(input, SIZE, stdin);

    fifo1 = open(PATH1, O_RDWR);

    checkError(fifo1, 1);

    readbytes = read(fifo1, buffer, SIZE);
    char secreto1 [readbytes];
    strcpy(secreto1, buffer);

    printf("Se han leido %d bytes y el secreto 1 es: %s \n", readbytes, secreto1);

    printf("Pulsar una tecla para comenzar el ejercicio 2.\n");
    fgets(input, SIZE, stdin);

    checkError(mkfifo(PATH2, S_IRWXU), 2);

    fifo2 = open(PATH2, O_RDWR);
    checkError(fifo2, 3);

    write(fifo2, secreto1, readbytes);

    readbytes = read(fifo1, buffer, SIZE);
    char secreto2 [readbytes];
    strcpy(secreto2, buffer);

    printf("El secreto 2 es: %s \n", secreto2);

    readbytes = write(fifo2, secreto2, readbytes);
   
    printf("Pulsar una tecla para comenzar el ejercicio 3.\n");
    fgets(input, SIZE, stdin);

    id_cola = msgget(CLAVE, 0666);
    checkError(id_cola, 4);

    err = msgrcv(id_cola, (struct msgbuf*) &msg, sizeof (msg.id) + sizeof (msg.data), 0, 0);

    checkError(err, 5);

    printf("El secreto 3 es <%ld> y el secreto 4 es %s\n", msg.id, msg.data);

    printf("Pulsar una tecla para comenzar el ejercicio 4.\n");
    fgets(input, SIZE, stdin);

    id_cola = msgget(CLAVE, 0666 | IPC_CREAT);
    checkError(id_cola, 6);

    char secretos3y4[sizeof (msg.data)*2];

    sprintf(secretos3y4, "<%ld>%s", msg.id, msg.data);
    strcpy(msg.data, secretos3y4);

    err = msgsnd(id_cola, (struct msgbuf*) &msg, sizeof (msg.id) + sizeof (msg.data), 0);
    checkError(err, 7);

    printf("Pulsar una tecla para comenzar el ejercicio 5.\n");
    fgets(input, SIZE, stdin);

    id_cola = msgget(CLAVE, 0666);
    checkError(id_cola, 4);
    
    err = msgrcv(id_cola, (struct msgbuf*) &msg, sizeof (msg.data), getpid(), 0);
    checkError(err, 5);

    printf("El secreto 6 es: %s\n", msg.data);

    printf("Pulsar una tecla para comenzar el ejercicio 6.\n");
    fgets(input, SIZE, stdin);

    msg.id = atoi(argv[1]);
    err = msgsnd(id_cola, (struct msgbuf*) &msg, sizeof (msg.id) + sizeof (msg.data), 0);
    checkError(err, 7);
    
    printf("Pulsar una tecla para comenzar el ejercicio 7.\n");
    fgets(input, SIZE, stdin);

    err = msgsnd(id_cola, (struct msgbuf*) &msg, sizeof (msg.id) + sizeof (msg.data), 0);
    checkError(err, 7);
    sleep(1);
    unlink(PATH1);
    unlink(PATH2);
    err = msgctl(id_cola, IPC_RMID, (struct msqid_ds*) NULL);
    checkError(err, 8);

    printf("Saliendo del programa...\n");
    //fgets(input, SIZE, stdin);

    return (EXIT_SUCCESS);
}

void checkError(int err, int type) {
    char error [SIZE];
    if (err == -1) {
        switch (type) {
            case 1: strcpy(error, "abriendo la fifo1");
                break;
            case 2: strcpy(error, "creando la fifo2");
                break;
            case 3: strcpy(error, "abriendo la fifo2");
                break;
            case 4: strcpy(error, "abriendo la cola de mensajes");
                break;
            case 5: strcpy(error, "leyendo de la cola de mensajes");
                break;
            case 6: strcpy(error, "creando la cola de mensajes");
                break;
            case 7: strcpy(error, "escribiendo en la cola de mensajes");
                break;
            case 8: strcpy(error, "destruyendo al cola de mensajes");
                break;
            default: strcpy(error, "desconocido");
                break;
        }

        printf("Se ha producido un error %s. Saliendo del programa...\n", error);
        exit(0);
    }
}