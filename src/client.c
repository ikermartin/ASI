
/* 
 * File:   client.c
 * Author: iker
 *
 * Created on 14 de mayo de 2017, 17:54
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

#define BUFFSIZE 256
#define CPORT 3000
#define SPORT 3001

/*
   if(<0)
    {
        printf("Error . Saliendo...\n");
        exit(0);
    }

 */


int main(int argc, char** argv) {

    int s2, s3;
    int fsd, sd, fd, addrlen;
    char input[BUFFSIZE];
    char buffer[BUFFSIZE];

    printf("Practica 4 parte I.\n--------------------\nPulsa una tecla para ejecutar el ejercicio 1.\n");
    fgets(input, BUFFSIZE, stdin);

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(CPORT);

    if ((fsd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }
    printf("Socket creado.\n");
    if (connect(fsd, (struct sockaddr*) &addr, sizeof (addr)) < 0) {
        printf("Error estableciendo conexión. Saliendo...\n");
        exit(0);
    }
    printf("Conexión establecida\n");

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 2.\n");
    fgets(input, BUFFSIZE, stdin);

    printf("Leyendo dato...\n");
    if (recv(fsd, buffer, BUFFSIZE, 0) < 0) {
        printf("Error de lectura. Saliendo...\n");
        exit(0);
    }
    s2 = atoi(buffer + 1);
    printf("Dato recibido %d -- %s\n", s2, buffer);

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 3.\n");
    fgets(input, BUFFSIZE, stdin);

    printf("Enviando dato...\n");
    if (send(fsd, buffer, strlen(buffer), 0) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }

    printf("Leyendo dato...\n");
    if (recv(fsd, buffer, BUFFSIZE, 0) < 0) {
        printf("Error de lectura. Saliendo...\n");
        exit(0);
    }
    s3 = atoi(buffer + 1);
    printf("Dato recibido %d\n", s3);

    printf("Enviando dato...\n");
    if (send(fsd, buffer, strlen(buffer), 0) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }
    sleep(5);
    printf("Cerrando socket.\n");
    close(fsd);

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 4.\n");
    fgets(input, BUFFSIZE, stdin);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }

    //Actualizamos el puerto al que se conectará el servidor 
    addr.sin_port = htons(SPORT);
    //memset(&server,0,sizeof(server)); Para qué?

    if (bind(sd, (struct sockaddr*) &addr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error en bind. Saliendo...\n");
        exit(0);
    }

    printf("Bind correcto. Iniciando la espera de conexión...\n");

    if (listen(sd, 10) < 0) {
        printf("Error escuchando en el socket . Saliendo...\n");
        exit(0);
    }
    addrlen = sizeof (struct sockaddr_in);
    if (fd = accept(sd, (struct sockaddr*) &addr, &addrlen) < 0) {
        printf("Error aceptando conexión . Saliendo...\n");
        exit(0);
    } else {
        printf("Conexión establecida\n");
    }



    sleep(30);
    return (EXIT_SUCCESS);
}

