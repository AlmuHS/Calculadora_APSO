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

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

int fin = 0;

void llega_fin(){
	fin = 1;
}

int main(){
	int fifo_motor, fifo_op2, fifo_operador;
	float num1, num2, resultado;
	int operador;
	
	fifo_motor=open("fifo_motor", O_WRONLY);
	fifo_op2=open("fifo_op2", O_RDONLY);
	fifo_operador=open("fifo_operador", O_RDONLY);
	
	signal(9, llega_fin);
	
	do{
		//Leemos el primer operando
		while(read(2, &num1, sizeof(num1)) == 0);
		
		//Leemos el segundo operando
		while(read(fifo_op2, &num2, sizeof(num2)) == 0);
	
		//Leemos el operador
		while(read(fifo_operador, &operador, sizeof(operador)) == 0);

		
		//Realizamos la operacion
		switch(operador){
			case 1:
				resultado = num1 + num2;
			break;
			
			case 2:
				resultado = num1 - num2;
			break;
			
			case 3:
				resultado = num1 * num2;
			break;
			
			case 4:
				resultado = num1 / num2;
			break;
		}
		
		//Pasamos el resultado al proceso interfaz
		write(fifo_motor, &resultado, sizeof(resultado));		
	}while (fin == 0);
	
	close(fifo_op2);
	close(fifo_operador);
	
	exit(0);
	
	return num2;
}
