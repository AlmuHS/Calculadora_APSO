/*Copyright 2016 2018 Almudena Garcia Jurado-Centurion

This file is part of Calculadora_APSO.

Calculadora_APSO is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Calculadora_APSO is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Calculadora_APSO.  If not, see <http://www.gnu.org/licenses/>.*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/wait.h>

#include "comunes.h"

//Variable control, usada para indicar la finalizacion del programa
int fin = 0;

//Funcion asociada a la senal de fin, que activa la variable de control
void llega_fin(){
	fin = 1;
}

int main(){

	//Identificadores de procesos y estructuras de comunicacion
	int pid_motor, id_cola, tubomotor[2];

	//Clave de la cola de mensajes, que comunica con el proceso principal
	key_t clave_cola;

	//Struct usado para guardar la informacion de la cola de mensajes
	struct numero num1;
	
	//Abrimos la cola, que recibira el primer operador del proceso principal
	clave_cola = ftok("./Makefile", 1024);
	id_cola = msgget(clave_cola, 0600);
	
	//Creamos la pipe tubomotor, que usaremos para enviar el dato leido al proceso motor
	pipe(tubomotor);
	
	//Creamos el proceso hijo motor
	pid_motor = fork();
	if(pid_motor == 0){

		//Reubicamos la posicion de lectura de la pipe en el canal 2
		close(2);
		dup(tubomotor[0]);//La posicion de lectura es la ubicada en la posicion 0 del array

		execl("motor", "motor", NULL);
	}
	
	//Nos preparamos para recibir señal de fin
	signal(30, llega_fin);

	
	do{
		//Leemos el primer operando del proceso principal, desde la cola de mensajes
		msgrcv(id_cola, (struct msgbuf *) &num1, sizeof(num1) - sizeof(long), 1, 0);
		
		//Pasamos el primer operando al motor
		write(tubomotor[1], &num1.num, sizeof(num1.num));

	//Repetimos el proceso hasta recibir la senal de fin
	}while(fin == 0);
	
	//Una vez recibida la senal de fin, iniciamos la finalizacion del programa

	//Transmitimos la senal al proceso motor
	kill(pid_motor, 9);
	
	//Esperamos a que el proceso motor finalice
	wait(&pid_motor);

	//Finalizamos el programa	

	return num1.num;
}
