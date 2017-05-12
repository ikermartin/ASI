/* 
 * File:   main.c
 * Author: iker
 *
 * Created on 11 de mayo de 2017, 18:52
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
//#include <sys/stat.h>
//#include <sys/types.h>


#define SIZE 256

int main(int argc, char** argv) {

    int pid1, pid2, i;
    int filedes[2];
    int readBytes;
    int status;
    char buffer [SIZE];
    char input [SIZE];


    printf("Prueba de comunicación entre procesos mediante pipes. \nEscribe un mensaje para continuar...\n");
    fgets(buffer, SIZE, stdin);
    printf("Mensaje leido: %s\n", buffer);

    pipe(filedes);

    if ((pid1 = fork()) == 0) //hijo1: lee de pipe
    {
        int bytes = 0;
        printf("\tHijo 1 creado.\n");

        //Lectura de varios caracteres del pipe

        for (i = 0; i < 10; i++) {
            bytes = read(filedes[0], (input + i), 1);
            readBytes = readBytes + bytes;
            printf("\tSe ha leido: %c\n", input[i]);
            fflush(stdout);
        }
        printf("\tSe han leido %d bytes del pipe que indican lo siguiente:\n- %s\n", readBytes, input);
       // fflush(stdout);


        //Lectura de 5 caracteres consecutivos
        /*
        char readC;
        for (i = 0; i < 5; i++) {
            readBytes=read(filedes[0], &readC, 1);
            printf("Se han leido %d bytes. Valor-> %c\n",readBytes,readC);
            fflush(stdout);
        }*/

        while (1) {
            pause();
        }
    } else if ((pid2 = fork()) == 0) // hijo2: escribe en pipe 
    {
        printf("\t\tHijo 2 creado\n");

        //Escritura de la entrada por teclado en el pipe

        for (i = 0; i < 10; i++) {
            printf("\t\tSe va a escribir: %c\n", buffer[i]);
            write(filedes[1], (buffer + i), 1);
            sleep(1);
        }

        //Escritura de 5 caracteres consecutivos
        /*
        char c = 'A';
        for (i = 0; i < 5; i++) {
            write(filedes[1], &c, 1);
            c++;
            sleep(2);
        }*/

        while (1) {
            pause();
        }

    } else // padre
    {
        printf("Padre a la espera\n");

        sleep(30);

        printf("Cuenta atrás terminada. El programa padre va a matar a los hijos y finalizar...\n");

        kill(pid1, SIGUSR1);
        waitpid(pid1, &status, 0);

        kill(pid2, SIGUSR1);
        waitpid(pid2, &status, 0);

    }

    return (EXIT_SUCCESS);
}
