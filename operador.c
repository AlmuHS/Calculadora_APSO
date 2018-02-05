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

//Variable control para indicar fin del programa
int fin = 0;

//Funcion asociada a la senal de fin, que activa la variable control
void llega_fin(){
	fin = 1;
}

int main(){

	//Identificadores de las estructuras de comunicacion
	int fifo_operador, id_cola;

	//Clave usada para abrir la cola
	key_t clave_cola;

	//Struct para guardar la informacion leida desde la cola
	struct operacion op;
	
	//Abrimos la cola, con la que leeremos el operador enviado por el proceso principal
	clave_cola = ftok("./Makefile", 1024);
	id_cola = msgget(clave_cola, 0600);
	
	//Nos preparamos para recibir señal de fin
	signal(16, llega_fin);
	
	do{
		//Leemos el operador de la cola
		msgrcv(id_cola, (struct msgbuf *) &op, sizeof(op) - sizeof(long), 3, 0);
		
		
		//Pasamos el operador al motor, mediante la fifo preparada para ello
		fifo_operador = open("fifo_operador", O_WRONLY);
		write(fifo_operador, &op.opcion, sizeof(op.opcion));

	//Repetimos el proceso hasta recibir la senal de fin	
	}while(fin == 0);

	
	return op.opcion;
}
