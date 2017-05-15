
/* 
 * File:   client.c
 * Author: iker
 *
 * Created on 14 de mayo de 2017, 17:54
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

#define BUFFSIZE 256
#define PORT1 3000
#define PORT2 3001
#define MENSAJE "HOLA!"

int main(int argc, char** argv) {

    int s2, s3, s5;
    int fsd, sd, fd, addrlen;
    char input[BUFFSIZE];
    char buffer[BUFFSIZE];

    printf("Practica 4 parte I.\n--------------------\nPulsa una tecla para ejecutar el ejercicio 1.\n");
    fgets(input, BUFFSIZE, stdin);

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT1);
    addr.sin_addr.s_addr = INADDR_ANY;

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

    // EJERCICIO 2   

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 2.\n");
    fgets(input, BUFFSIZE, stdin);

    printf("Leyendo dato...\n");
    if (recv(fsd, buffer, BUFFSIZE, 0) < 0) {
        printf("Error de lectura. Saliendo...\n");
        exit(0);
    }
    s2 = atoi(buffer + 1);
    printf("Dato recibido %d \n", s2);

    // EJERCICIO 3   

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
    sleep(1);
    printf("Cerrando socket.\n");
    close(fsd);

    // EJERCICIO 4    

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 4 (ejecutar aquí primero).\n");
    fgets(input, BUFFSIZE, stdin);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }

    //Actualizamos el puerto al que se conectará el servidor 

    memset(&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT2);
    addr.sin_addr.s_addr = INADDR_ANY;

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
    if ((fd = accept(sd, (struct sockaddr*) &addr, &addrlen)) < 0) {
        printf("Error aceptando conexión . Saliendo...\n");
        exit(0);
    } else {
        printf("Conexión establecida\n");
    }

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 5.\n");
    fgets(input, BUFFSIZE, stdin);

    // EJERCICIO 5

    printf("Leyendo dato...\n");
    if (recv(fd, buffer, BUFFSIZE, 0) < 0) {
        printf("Error de lectura. Leido %s Saliendo...\n", buffer);
        exit(0);
    }
    s5 = atoi(buffer + 1);
    printf("Dato recibido %d\n", s5);

    printf("Enviando dato...\n");
    if (send(fd, buffer, strlen(buffer), 0) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }
    sleep(2);
    printf("Cerrando socket.\n");
    close(sd);


    // EJERCICIO 6

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 6.\n");
    fgets(input, BUFFSIZE, stdin);

    int sdUDP;
    struct sockaddr_in myaddr, monaddr;

    bzero((char*) &myaddr, sizeof (struct sockaddr_in));
    //bzero((char*) &svaddr, sizeof (struct sockaddr_in));

    monaddr.sin_family = AF_INET;
    monaddr.sin_port = htons(PORT1);
    monaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sdUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }

    printf("Socket UDP %d creado.\n", sdUDP);

    //unimos el socket a nuestra dirección en un puerto random

    if (bind(sdUDP, (struct sockaddr*) &myaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error en bind. Saliendo...\n");
        exit(0);
    }

    printf("Bind creado Puerto del cliente: %d.\n", myaddr.sin_port);
    sleep(1);
    strcpy(buffer, MENSAJE);
    printf("Se va a enviar el mensaje: %s\n", buffer);

    if (sendto(sdUDP, buffer, strlen(buffer), 0, (struct sockaddr*) &monaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }

    printf("Mensaje enviado.\n");

    // EJERCICIO 7

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 7. (Ejecutar aquí primero)\n");
    fgets(input, BUFFSIZE, stdin);

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT2);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Actualizamos el socket
    if ((sdUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }

    printf("Socket UDP %d creado.\n", sdUDP);

    if (bind(sdUDP, (struct sockaddr*) &myaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error en bind. Saliendo...\n");
        exit(0);
    }
    printf("Bind creado Puerto del cliente: %d.\n", myaddr.sin_port);

    sleep(2);
    addrlen = sizeof (struct sockaddr_in);

    if (recvfrom(sdUDP, buffer, BUFFSIZE, 0, (struct sockaddr*) &monaddr, &addrlen) < 0) {
        printf("Error recibiendo datos. Leido %s Saliendo...\n", buffer);
        exit(0);
    }

    printf("Mensaje recibido: %s\n", buffer);

    if (sendto(sdUDP, buffer, strlen(buffer), 0, (struct sockaddr*) &monaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }

    printf("Mensaje enviado correctamene.\n");

    sleep(10);
    return (EXIT_SUCCESS);
}

