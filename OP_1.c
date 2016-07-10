//Copyright 2016 Almudena Garcia Jurado-Centurion

/*This file is part of Calculadora_APSO.

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

int fin = 0;

void llega_fin(){
	fin = 1;
}

int main(){
	int pid_motor, id_cola, tubomotor[2];
	key_t clave_cola;
	struct numero num1;
	
	//Abrimos la cola
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	
	//Creamos la pipe
	pipe(tubomotor);
	
	//Creamos el proceso hijo motor
	pid_motor=fork();
	if(pid_motor==0){
		close(2);
		dup(tubomotor[0]);
		execl("motor", "motor", NULL);
	}
	
	//Nos preparamos para recibir señal de fin
	signal(30, llega_fin);

	
	do{
		msgrcv(id_cola, (struct msgbuf *) &num1, sizeof(num1) - sizeof(long), 1, 0);
		
		//Pasamos el primer operando al motor
		write(tubomotor[1], &num1.num, sizeof(num1.num));
	}while(fin == 0);
	
	kill(pid_motor, 9);
	
	wait(&pid_motor);
	
	return num1.num;
}
