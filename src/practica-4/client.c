
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
#include <arpa/inet.h>


#define BUFFSIZE 256
#define PORT1 3000
#define PORT2 3001
#define MENSAJE "HOLA!"

int main(int argc, char** argv) {

    int s2, s3, s5;
    int fdc, sd, fd, addrlen;
    char input[BUFFSIZE];
    char buffer[BUFFSIZE];


    // Ejercicio 1. Aquí el programa client actúa como cliente e inicia una conexión TCP hacia el monitor

    printf("Practica 4 parte I.\n--------------------\nPulsa una tecla para ejecutar el ejercicio 1.\n");
    fgets(input, BUFFSIZE, stdin);

    struct sockaddr_in server_in;

    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(PORT1);
    server_in.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((fdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }
    printf("Socket creado.\n");

    if (connect(fdc, (struct sockaddr*) &server_in, sizeof (struct sockaddr_in)) < 0) {
        printf("Error estableciendo conexión. Saliendo...\n");
        exit(0);
    }
    printf("Conexión establecida\n");

    // EJERCICIO 2   Se mantiene la conexión anterior

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 2.\n");
    fgets(input, BUFFSIZE, stdin);

    printf("Leyendo dato...\n");
    if (recv(fdc, buffer, BUFFSIZE, 0) < 0) {
        printf("Error de lectura. Saliendo...\n");
        exit(0);
    }
    s2 = atoi(buffer + 1);
    printf("Dato recibido %d \n", s2);

    // EJERCICIO 3   

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 3.\n");
    fgets(input, BUFFSIZE, stdin);

    printf("Enviando dato...\n");
    if (send(fdc, buffer, strlen(buffer), 0) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }

    printf("Leyendo dato...\n");
    if (recv(fdc, buffer, BUFFSIZE, 0) < 0) {
        printf("Error de lectura. Saliendo...\n");
        exit(0);
    }
    s3 = atoi(buffer + 1);
    printf("Dato recibido %d\n", s3);

    printf("Enviando dato...\n");
    if (send(fdc, buffer, strlen(buffer), 0) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }
    sleep(1);
    printf("Cerrando socket.\n");
    close(fdc);

    // EJERCICIO 4      En este ejercicio el programa client.c actúa como servidor, 
    //                  y es el monitor quien inicia la conexión TCP

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 4 (ejecutar aquí primero).\n");
    fgets(input, BUFFSIZE, stdin);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }

    struct sockaddr_in client_in;

    memset(&server_in, 0, sizeof (server_in));

    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(PORT2);
    server_in.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr*) &server_in, sizeof (struct sockaddr_in)) < 0) {
        printf("Error en bind. Saliendo...\n");
        exit(0);
    }

    printf("Bind correcto. Iniciando la espera de conexión...\n");

    if (listen(sd, 10) < 0) {
        printf("Error escuchando en el socket . Saliendo...\n");
        exit(0);
    }
    addrlen = sizeof (struct sockaddr_in);
    if ((fd = accept(sd, (struct sockaddr*) &client_in, &addrlen)) < 0) {
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
    close(fd);
    close(sd);


    // EJERCICIO 6 En este ejercicio, el prorgama envía al monitor un dato, por lo que 
    // suponemos que actúa, en principio, como un cliente UDP

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 6.\n");
    fgets(input, BUFFSIZE, stdin);

    int fdcUDP, fdUDP;
    struct sockaddr_in myaddr, monaddr;


    bzero((char*) &myaddr, sizeof (struct sockaddr_in));
    bzero((char*) &monaddr, sizeof (struct sockaddr_in));

    monaddr.sin_family = AF_INET;
    monaddr.sin_port = htons(PORT1);
    monaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /*
     * Esto sería lo ideal si tuviéramos un servicio asignado a esta práctica en el 
     * directorio /etc/services. Buscaría en dicho fichero el puerto necesario para
     * configurar la conexión. Como no lo tenemos, dejamos el puerto 0 por defecto.
     * También se puede elegir otro puerto que no sea 3000 (lo usa el monitor para recibir) 
     * ni el 3001 (es necesario en el siguiente ejercicio) y no lo use otro programa
     * 
        struct servent *sp;
        sp=getservbyname("UDP_SERVICE","udp");
        if(sp==NULL){
            printf("Error getservbyname...\n");
            exit(0);
        }
        myaddr.sin_port=sp->s_port;
     */
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((fdcUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }
    printf("Socket UDP %d creado.\n", fdcUDP);

    if (bind(fdcUDP, (struct sockaddr*) &myaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error en bind. Saliendo...\n");
        exit(0);
    }
    printf("Bind creado Puerto del cliente: %d.\n", myaddr.sin_port);

    sleep(1);
    strcpy(buffer, MENSAJE);
    printf("Se va a enviar el mensaje: %s\n", buffer);

    if (sendto(fdcUDP, buffer, strlen(buffer), 0, (struct sockaddr*) &monaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }
    printf("Mensaje enviado.\n");

    // EJERCICIO 7 En este caso, recibimos los datos desde el monitor. Se puede decir que actuamos de "servidor" UDP

    printf("--------------------\nPulsa una tecla para ejecutar el ejercicio 7. (Ejecutar aquí primero)\n");
    fgets(input, BUFFSIZE, stdin);

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT2);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Creamos el segundo socket para recibir la información

    if ((fdUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("Error creando socket. Saliendo...\n");
        exit(0);
    }
    printf("Socket UDP %d creado.\n", fdUDP);

    if (bind(fdUDP, (struct sockaddr*) &myaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error en bind. Saliendo...\n");
        exit(0);
    }
    printf("Bind creado Puerto del cliente: %d.\n", myaddr.sin_port);

    sleep(2);
    addrlen = sizeof (struct sockaddr_in);

    if (recvfrom(fdUDP, buffer, BUFFSIZE, 0, (struct sockaddr*) &monaddr, &addrlen) < 0) {
        printf("Error recibiendo datos. Leido %s Saliendo...\n", buffer);
        exit(0);
    }
    printf("Mensaje recibido: %s\n", buffer);

    if (sendto(fdUDP, buffer, strlen(buffer), 0, (struct sockaddr*) &monaddr, sizeof (struct sockaddr_in)) < 0) {
        printf("Error de escritura. Saliendo...\n");
        exit(0);
    }
    printf("Mensaje enviado correctamente.\n");

    sleep(10);

    close(fdUDP);
    close(fdcUDP);
    return (EXIT_SUCCESS);
}

