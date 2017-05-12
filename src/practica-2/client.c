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

#define SIZE 256
#define PATH1 "/tmp/fifo_monitor_1"
#define PATH2 "/tmp/fifo_monitor_2"

int main(int argc, char** argv) {

    int fifo1, fifo2, readbytes;
    char buffer[SIZE];
    char lectura[SIZE];

    if (argc != 2) {
        printf("Error, número de argumentos de entrada incorrecto. Saliendo del programa...\n");
        exit(0);
    }

    printf("Pulsa una tecla para comenzar el ejercicio 1.\n");
    fgets(lectura, SIZE, stdin);

    fifo1 = open(PATH1, O_RDWR);

    if (fifo1 == -1) {
        printf("Error abriendo fifo1. Saliendo del programa...\n");
        exit(0);
    }

    readbytes = read(fifo1, buffer, SIZE);
    char secreto1 [readbytes];
    strcpy(secreto1, buffer);

    printf("Se han leido %d bytes y el secreto 1 es: %s \n", readbytes,secreto1);

    printf("Pulsar una tecla para comenzar el ejercicio 2.\n");
    fgets(lectura, SIZE, stdin);

    if ((mkfifo(PATH2, S_IRWXU)) == -1) {
        printf("Error creando fifo2. Saliendo...\n");
        exit(0);
    }

    fifo2 = open(PATH2, O_RDWR);

    if (fifo2 == -1) {
        printf("Error abriendo fifo2. Saliendo del programa...\n");
        exit(0);
    }

    write(fifo2, secreto1, readbytes);

    readbytes = read(fifo1, buffer, SIZE);
    char secreto2 [readbytes];
    strcpy(secreto2, buffer);

    printf("El secreto 2 es: %s \n", secreto2);

    readbytes = write(fifo2, secreto2, readbytes);
    printf("Se han escrito %d bytes en fifo2\n", readbytes);


    
    
    
    printf("Pulsa una tecla para salir.\n");
    fgets(lectura, SIZE, stdin);


    return (EXIT_SUCCESS);
}

