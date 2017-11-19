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

#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>


#include "comunes.h"

int fin = 0;

void llega_fin(){
	fin = 1;
}


int main(){
	int fifo_op2, id_cola;
	key_t clave_cola;
	struct numero num2;
	
	//Abrimos la cola
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	
	//Nos preparamos para recibir señal de fin
	signal(31, llega_fin);
	
	do{
		msgrcv(id_cola, (struct msgbuf *) &num2, sizeof(num2) - sizeof(long), 2, 0);
		
		//Pasamos el segundo operando al motor
		fifo_op2=open("fifo_op2", O_WRONLY);
		write(fifo_op2, &num2.num, sizeof(num2.num));
	}while(fin == 0);
	
	return num2.num;
}
