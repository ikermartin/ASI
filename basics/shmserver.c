
/* 
 * File:   shmserver.c
 * Author: iker
 *
 * Created on 13 de mayo de 2017, 11:39
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define KEYPATH "/usr/bin/"
#define SHMSIZE 2048
#define BUFFSIZE 256
#define SEMOP(id,st_op) (semop(id,st_op,sizeof(st_op)/sizeof(struct sembuf)))

void checkError(int err, int type);
void swapCaps(char * original);

int main(int argc, char** argv) {
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
    printf("Proceso servidor. Creando zona de memoria compartida con clave %ld\n", key1);

    shmid = shmget(key1, SHMSIZE, IPC_CREAT | 0660);
    checkError(shmid, 1);

    segment = shmat(shmid, NULL, 0);
    checkError(segment, 2);

    printf("Zona de memoria compartida creada y servidor unido.\n");

    key2 = ftok(KEYPATH, 25);
    printf("Creando un semáforo con clave: %ld\n", key2);
    semid = semget(key2, 1, IPC_CREAT | 0660);
    checkError(semid, 3);
    err = semctl(semid, 0, SETVAL, 1);
    checkError(err, 4);
    sleep(10);

    do {
        SEMOP(semid, down);
        strcpy(input, segment);
        SEMOP(semid, up);

        if (strlen(input) == 0) {
            printf("Aun no hay un mensaje en memoria.\n");
        } else {
            printf("Leido el siguiente mensaje de longitud %ld:\n- %s\n", strlen(input), input);
            swapCaps(input);
            printf("Mensaje traducido:\n- %s\n", input);

            SEMOP(semid, down);
            strcpy(segment, input);
            SEMOP(semid, up);
            printf("Mensaje traducido escrito en memoria. Esperando 15 seg para liberar recursos...\n");
        }
        sleep(2);

    } while (strlen(input) == 0);

    sleep(15);

    printf("Se procede a liberar los recursos empleados. \n");
    err = shmdt(segment);
    checkError(err, 6);

    err = shmctl(shmid, IPC_RMID, (struct shmid_ds*) NULL);
    checkError(err, 5);

    err = semctl(semid, 0, IPC_RMID);
    checkError(err, 7);
    return (EXIT_SUCCESS);
}

void checkError(int err, int type) {
    char error [BUFFSIZE];
    if (err == -1) {
        switch (type) {
            case 1: strcpy(error, "creando la zona de memoria compartida");
                break;
            case 2: strcpy(error, "uniéndose a zona de memoria compartida");
                break;
            case 3: strcpy(error, "creando un semáforo");
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

void swapCaps(char * original) {
    printf("Hay %ld caracteres por traducir.\n", strlen(original));
    for (int i = 0; i < strlen(original); i++) {
        if (original[i] >= 65 && original[i] <= 90) {
            original[i] = original[i] + 32;
        } else if (original[i] >= 97 && original[i] <= 122) {
            original[i] = original[i] - 32;
        } else {
            original[i] = original[i];
        }
    }
}