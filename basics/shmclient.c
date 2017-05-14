/* 
 * File:   shmclient.c
 * Author: iker
 *
 * Created on 13 de mayo de 2017, 11:39
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


#define KEYPATH "/usr/bin/"
#define SHMSIZE 2048
#define BUFFSIZE 256
#define SEMOP(id,st_op) (semop(id,st_op,sizeof(st_op)/sizeof(struct sembuf)))

void quitJump(char *string);
void checkError(int err, int type);

/*
 * 
 */
int main(int argc, char** argv) {

    int pid1, pid2, status;
    int err, shmid, semid;
    char input[BUFFSIZE];
    char modified[BUFFSIZE];
    char *segment;
    long key1, key2;

    struct sembuf up [1] = {
        {0, 1, 0}
    };
    struct sembuf down [1] = {
        {0, -1, 0}
    };

    key1 = ftok(KEYPATH, 10);

    printf("Programa cliente. Tratando de unirse a la memoria compartida con clave: %ld\n", key1);
    shmid = shmget(key1, SHMSIZE, 0660);
    checkError(shmid, 1);
    segment = shmat(shmid, NULL, 0660);
    checkError(segment, 2);
    key2 = ftok(KEYPATH, 25);
    printf("Accediendo al semáforo con clave: %ld\n", key2);
    semid = semget(key2, 1, 0660);
    checkError(semid, 3);

    printf("Escribir mensaje para guardar en memoria compartida...\n - ");
    fgets(input, BUFFSIZE, stdin);
    quitJump(input);

    printf("Cliente unido a segmento de memoria.\n");

    if ((pid1 = fork()) == 0) { //hijo1 que escribira en la memoria compartida
        printf("\tHijo1 con PID: %d\n - ", getpid());
        printf("\t Se va a escribir <%s> en memoria.\n", input);

        SEMOP(semid, down);
        strcpy(segment, input);
        sleep(1);
        SEMOP(semid, up);
        printf("\t Mensaje <%s> escrito en memoria.\n", input);

        while (1) {
            pause();
        }
    } else if ((pid2 = fork()) == 0) { //hijo2 leerá de la memoria compartida

        printf("\t\tHijo2 con PID: %d\n - ", getpid());
        printf("\t\tLeyendo mensaje en memoria...\n");

        do {
            SEMOP(semid, down);
            strcpy(modified, segment);
            SEMOP(semid, up);

            if (strcmp(input, modified) == 0 || strlen(modified) == 0) {
                printf("\t\tMensaje no traducido aún...\n");
            } else {
                printf("\t\t Mensaje traducido: <%s> \n", modified);
            }
            sleep(2);
        } while (strcmp(input, modified) == 0 || strlen(modified) == 0);

        while (1) {
            pause();
        }

    } else { //padre

        printf("Padre con PID: %d\n - ", getpid());
        sleep(30);

        printf("Temporización finalizada. Liberando recursos y saliendo...\n");

        err = shmdt(segment);
        checkError(err, 6);

        kill(pid1, SIGUSR1);
        waitpid(pid1, &status, 0);

        kill(pid2, SIGUSR1);
        waitpid(pid1, &status, 0);
    }





    sleep(5);
    err = shmdt(segment);

    return (EXIT_SUCCESS);
}

void quitJump(char *string) {
    if (string[strlen(string) - 1] == '\n') {
        string[strlen(string) - 1] = '\0';
    }
}

void checkError(int err, int type) {
    char error [BUFFSIZE];
    if (err == -1) {
        switch (type) {
            case 1: strcpy(error, "obteniendo la id de la zona de memoria compartida");
                break;
            case 2: strcpy(error, "uniéndose a zona de memoria compartida");
                break;
            case 3: strcpy(error, "obteniendo la id del semáforo");
                break;
            case 4: strcpy(error, "inicializando el semáforo");
                break;
            case 5: strcpy(error, "liberando la zona de memoria compartida");
                break;
            case 6: strcpy(error, "desatándose de la zona de memoria");
                break;
            case 7: strcpy(error, "eliminando el semáforo");
                break;
            default: strcpy(error, "desconocido");
                break;
        }

        printf("Se ha producido un error %s. Saliendo del programa...\n", error);
        exit(0);
    }
}