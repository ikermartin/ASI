/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   client.c
 * Author: docencia
 *
 * Created on March 8, 2017, 8:29 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <sys/types.h> //waitpid
#include <sys/wait.h> //waitpid
#include <pthread.h> //threads



/*
 *
 */

int pidMonitor, threads;
int seinales=0;


void echo();
void creaHilo();
void *func_hilo(void *arg);
void cuentaSeinales();
void echoCount();




int main(int argc, char** argv) {
    int pidhijo1,pidhijo2, status;
    
    int hilos;
    
    pidMonitor=atoi(argv[1]);
    printf("PidMonitor= %d \n", pidMonitor);
    
    signal(SIGUSR2,echo);
    
    
    printf("Padre con PID: %d\n",getpid());

    printf("Pulsar cualquier tecla para ejecutar el ejercicio 1\n");
    getchar();
    kill(pidMonitor,SIGUSR1);
    sleep(5);


    printf("Pulsar cualquier tecla para ejecutar el ejercicio 2\n");
    getchar();
    kill(pidMonitor,SIGUSR1);
    sleep(5);

    printf("Pulsar cualquier tecla para ejecutar el ejercicio 3\n");
    getchar();
    kill(pidMonitor,SIGUSR1);
    sleep(1);    
    
    if((pidhijo1=fork())==0) // código hijo 1
    {
        printf("\tHijo1 con PID: %d\n",getpid());
        signal(SIGUSR2, creaHilo);
	     
        while(1)
        {
            pause();
        }
    }
    
    else if((pidhijo2=fork())==0) // código hijo 2
    {
        printf("\tHijo2 con PID: %d\n",getpid());
		signal(SIGUSR1, cuentaSeinales);
		signal(SIGUSR2, echoCount);
        
        while(1)
        {
            pause();
        }
    }
    
    else    //código padre
    {     
      
        kill(pidMonitor,SIGUSR1);
        sleep(5);

		printf("Pulsar cualquier tecla para ejecutar el ejercicio 4\n");
		getchar();
		sleep(15);

		printf("Pulsar cualquier tecla para ejecutar el ejercicio 5\n");
		getchar();
		kill(pidMonitor,SIGUSR1);
		sleep(30);
        
        printf("Pulsar cualquier tecla para ejecutar el ejercicio 6\n");
        getchar();
		kill(pidMonitor,SIGUSR1);
		sleep(1);
	 
	 	//matamos a los hijos antes de finalizar

		kill(pidhijo1,SIGINT);
        waitpid(pidhijo1,&status,0);
        
        kill(pidhijo2,SIGINT);
        waitpid(pidhijo2,&status,0);
 
        exit(1);
    }
    
    return (EXIT_SUCCESS);
}

void echo()
{
    kill(pidMonitor,SIGUSR2);
    printf("\tSIGUSR2 recibida y eco enviado\n");
}

void creaHilo()
{
    pthread_t thd;
    pthread_create(&thd,NULL,(void*)func_hilo,NULL);
}

void *func_hilo(void *arg)
{
    int t=0;
    threads++;
    
    printf("\tNuevo hilo creado. Actualmente hay: %d \n", threads);
    
    do{
        if(t%5==0)
        {
            printf("\t\tAquí un thread que sigue vivo\n");
        }
        sleep(1);
        t++;
    }while(t<=10);
}

void cuentaSeinales()
{
	printf("\tSeñal SIGUSR1 recibida\n");
}

void echoCount()
{
	seinales++;    
	kill(pidMonitor,SIGUSR2);
    printf("\tSIGUSR2 recibida y eco enviado. Van %d señales\n", seinales);
}

