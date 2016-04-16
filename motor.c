#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	int fifo_motor, fifo_op2, fifo_operador;
	float num1, num2, resultado;
	int operador;
	
	fifo_motor=open("fifo_motor", O_WRONLY);
	fifo_op2=open("fifo_op2", O_RDONLY);
	fifo_operador=open("fifo_operador", O_RDONLY);
	
	
	//Leemos el primer operando
	read(2, &num1, sizeof(num1));
	//printf("Operando 1: %d\n", num1);
	
	//Leemos el segundo operando
	read(fifo_op2, &num2, sizeof(num2));
	//printf("Operando 2: %d\n", num2);
	
	//Leemos el operador
	read(fifo_operador, &operador, sizeof(operador));
	//printf("Operacion: %d\n", operador);
	
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
	write(2, &resultado, sizeof(resultado));
	
	close(fifo_op2);
	close(fifo_operador);
	
	unlink("fifo_op2");
	unlink("fifo_operador");
	
	return num2;
}