/*Copyright 2016 2017 2018 Almudena Garcia Jurado-Centurion

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

#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>


#include "comunes.h"

//Variable control, usada para indicar la finalizacion del programa
int fin = 0;

//Funcion asociada a la senal de fin, que activa la variable control
void llega_fin(){
	fin = 1;
}


int main(){
	
	//Identificadores de la fifo y la cola
	int fifo_op2, id_cola;

	//Clave usada para abrir la cola
	key_t clave_cola;

	//Struct usado para guardar la informacion procedente de la cola
	struct numero num2;
	
	//Abrimos la cola, que usaremos para recibir el operando procendente del proceso principal
	clave_cola = ftok("./Makefile", 1024);
	id_cola = msgget(clave_cola, 0600);
	
	//Nos preparamos para recibir señal de fin
	signal(31, llega_fin);
	
	do{
		//Leemos el operando, procedente de la cola proceso principal
		msgrcv(id_cola, (struct msgbuf *) &num2, sizeof(num2) - sizeof(long), 2, 0);
		
		//Pasamos el segundo operando al motor, usando la fifo preparada para ello
		fifo_op2 = open("fifo_op2", O_WRONLY);
		write(fifo_op2, &num2.num, sizeof(num2.num));

	//Repetimos el proceso hasta recibir la senal de fin
	}while(fin == 0);
	
	return num2.num;
}
