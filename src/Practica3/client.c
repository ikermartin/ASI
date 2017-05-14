
/* 
 * File:   client.c
 * Author: iker
 *
 * Created on 14 de mayo de 2017, 12:08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>


#define KEY 0x72477396L
#define SHMSIZE 1024
#define BUFFSIZE 256

#define SEMOP(id,st_op) (semop(id,st_op,sizeof(st_op)/sizeof(struct sembuf)))

/*
 * 
 */
int main(int argc, char** argv) {
    int s1, s2, s8, s10;
    int shmid, semid, err, offset;
    char *segment;
    char input[BUFFSIZE];
    char buffer[BUFFSIZE];

    printf("Práctica 4.\n---------------------\nPulse una tecla para ejecutar el ejercicio 1.\n");
    fgets(input, BUFFSIZE, stdin);

    shmid = shmget(KEY, SHMSIZE, 0666);
    if (shmid == -1) {
        printf("Error al acceder a shm Saliendo del programa.\n ");
        exit(0);
    }

    segment = shmat(shmid, NULL, 0);
    if (segment == -1) {
        printf("Error al engancharse a shm Saliendo del programa.\n ");
        exit(0);
    }

    s1 = *((int*) segment);
    offset = *((int*) segment + 1);

    printf("Secreto1: <%d> y offset: <%d>\n", s1, offset);

    char secreto2 [10];

    strcpy(secreto2, segment + offset);
    printf("Secreto2: %s \n", secreto2);
    sleep(3);
    shmdt(segment);

    printf("--------------------\nPulse una tecla para ejecutar el ejercicio 2.\n");
    fgets(input, BUFFSIZE, stdin);

    shmid = shmget(KEY, SHMSIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        printf("Error al acceder a shm Saliendo del programa.\n ");
        exit(0);
    }

    segment = shmat(shmid, NULL, 0);
    if (segment == -1) {
        printf("Error al engancharse a shm Saliendo del programa.\n ");
        exit(0);
    }
    sleep(1);

    strcpy(segment, secreto2);
    offset = atoi((secreto2 + 1)) + 16;
    sprintf(buffer, "<%d>", s1);
    printf("Escribiendo en MC cadena %s, con offset %d\n", buffer, offset);

    strcpy(segment + offset, buffer);
    shmdt(segment);
    printf("Desenganchado de memoria compartida.\n");

    printf("--------------------\nPulse una tecla para ejecutar el ejercicio 3.\n");
    fgets(input, BUFFSIZE, stdin);

    semid = semget(KEY, 1, IPC_CREAT | 0666);
    err = semctl(semid, 0, SETVAL, 396);
    printf("Semáforo creado e inicializado.\n");

    sleep(2);

    err = semctl(KEY, 0, IPC_RMID);

    printf("--------------------\nPulse una tecla para ejecutar el ejercicio 4.\n");
    fgets(input, BUFFSIZE, stdin);

    semid = semget(KEY, 2, IPC_CREAT | 0666);
    err = semctl(semid, 0, SETVAL, 1);
    err = semctl(semid, 1, SETVAL, 2);
     printf("Semáforo doble creado e inicializado.\n");

    sleep(5);
    printf("--------------------\nPulse una tecla para ejecutar el ejercicio 5.\n");
    fgets(input, BUFFSIZE, stdin);

    struct sembuf up [1] = {
        {0, 1, 0}
    };
    struct sembuf down [1] = {
        {0, -1, 0}
    };

    semid = semget(KEY, 1, 0666);
    printf("Id del semáforo obtenida.\n");
    shmid = shmget(KEY, SHMSIZE, 0666);
    segment = shmat(shmid, NULL, 0);
    printf("Unido a memoria compartida.\n");

    SEMOP(semid, down);
    printf("Dentro de la zona crítica.\n");
    s8 = *((int*) segment);
    printf("Secreto 8: <%d>\n", s8);
    *((int*) segment) = -s8;
    sleep(3);
    SEMOP(semid, up);
    printf("Fuera de la zona crítica.\n");

    sleep(5);
    printf("--------------------\nPulse una tecla para ejecutar el ejercicio 6.\n");
    fgets(input, BUFFSIZE, stdin);

    struct sembuf up2 [2] = {
        {0, 1, 0},
        {1, 1, 0}
    };
    struct sembuf down2 [2] = {
        {0, -1, 0},
        {1, -1, 0}
    };

    semid = semget(KEY, 2, 0666);
    printf("Id del semáforo doble obtenida.\n");
    shmid = shmget(KEY, SHMSIZE, 0666);
    segment = shmat(shmid, NULL, 0);
    printf("Unido a memoria compartida.\n");

    SEMOP(semid, down2);
    printf("Dentro de la zona crítica.\n");
    s10 = *((int*) segment);
    printf("Secreto 10: <%d>\n", s10);
    *((int*) segment) = -s10;
    sleep(3);
    SEMOP(semid, up2);
    printf("Fuera de la zona crítica.\n");

    sleep(5);
    printf("--------------------\nPulse una tecla para ejecutar el ejercicio 7.\n");
    fgets(input, BUFFSIZE, stdin);

    shmid = shmget(KEY, SHMSIZE, 0666);
    segment = shmat(shmid, NULL, 0);
    printf("Unido a memoria compartida.\n");
    semid = semget(KEY, 1, IPC_CREAT | 0666);
    err = semctl(semid, 0, SETVAL, 0);

    SEMOP(semid, down);
    printf("Dentro de la zona crítica.\n");
    printf("Liberando recursos.\n");
    err = shmctl(shmid, IPC_RMID, NULL);
    printf("Memoria eliminada.\n");
    err = semctl(semid, 0, IPC_RMID);
    printf("Semáforo eliminado. Saliendo.\n");

    return (EXIT_SUCCESS);
}

