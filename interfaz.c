/*Copyright 2016 2017 Almudena Garcia Jurado-Centurion

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

#include <stdio.h> //printf(), scanf()
#include <unistd.h> //fork(), execl();
#include <fcntl.h> //open()
#include <sys/types.h> //kill()
#include <sys/stat.h> //mkfifo()
#include <sys/msg.h> //colas de mensajes
#include <signal.h> //signal()
#include <sys/wait.h>

#include "comunes.h"

int op_seguir = 0;

void menu(float *num1, float *num2, int *operacion){
	
	//Si se realiza una nueva operacion, se pregunta por el primer numero
	if(op_seguir == 0){
		printf("Introduce un numero: ");
		scanf("%f", num1);
	}
	
	printf("Operacion\n");
	printf("1. Suma\n");
	printf("2. Resta\n");
	printf("3. Multiplicacion\n");
	printf("4. Division\n");
	printf("5. Salir\n");
	printf("6. Nueva operacion\n");
	do{
		printf("Selecciona operacion: ");
		scanf("%d", operacion);
	}while(*operacion < 1 || *operacion > 6);
	
	//Si se elige alguna operacion, se pregunta por el segundo numero
	if(*operacion != 5 && *operacion != 6){
		do{
			printf("Introduce otro numero: ");
			scanf("%f", num2);
		}while(*num2 == 0 && *operacion == 4);
	}
	else if(*operacion == 6) printf("Cache borrada\n\n");
}

int main(){
	float resultado; 
	int pid_op1, pid_op2, pid_operador;
	int fifo_motor;
	key_t clave_cola;
	int id_cola;
	char sig_operacion[4] = {'+', '-', 'x', '/'};

	
	struct operacion op;
	struct numero num1, num2;
	
	//Creamos la cola para pasar los parametros
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600 | IPC_CREAT);
	
	
	//Creamos el proceso hijo OP_1
	pid_op1=fork();
	if(pid_op1==0){
		execl("OP_1", "OP_1", NULL);
	}
	
	//Creamos el proceso hijo OP_2
	pid_op2=fork();
	if(pid_op2==0){
		execl("OP_2", "OP_2", NULL);
	}

	//Creamos el proceso hijo operador
	pid_operador=fork();
	if(pid_operador==0){
		execl("operador", "operador", NULL);
	}
	
	//Creamos la fifo del motor
	mkfifo("fifo_motor", 0777);
	mkfifo("fifo_op2", 0777);
	mkfifo("fifo_operador", 0777);	
	
	do{	
		//Indicamos el tipo de los parametros
		op.tipo=3;
		num1.tipo=1;
		num2.tipo=2;
		
		//Obtenemos los parametros
		op.opcion = 1;
		menu(&num1.num, &num2.num, &op.opcion);
		
		if(op.opcion != 5){
			
			//Si el usuario no selecciona nueva operacion, ocultamos la pregunta del primer numero
			if(op.opcion != 6) op_seguir = 1;
			//Si el usuario selecciona nueva operacion, activamos la pregunta del primer numero
			else op_seguir = 0;

			//Pasamos el primer operando al proceso OP_1
			msgsnd(id_cola, (struct msgbuf *) &num1, sizeof(num1) - sizeof(long), 0);
			
			//Pasamos el operador al proceso operador
			msgsnd(id_cola, (struct msgbuf *) &op, sizeof(op) - sizeof(long), 0);

			//Pasamos el segundo operando al proceso OP_2
			msgsnd(id_cola, (struct msgbuf *) &num2, sizeof(num2) - sizeof(long), 0);
			
			
			//Leemos el resultado del motor
			fifo_motor=open("fifo_motor", O_RDWR);
			while(read(fifo_motor, &resultado, sizeof(resultado)) == 0);
			
			
						
			//Si se ha realizado alguna operacion, mostramos el resultado
			if(op_seguir == 1) printf("%f %c %f = %f\n\n", num1.num, sig_operacion[op.opcion-1], num2.num, resultado);
			
			//copiamos el resultado en op1
			num1.num=resultado;
		}
		
		
	}while(op.opcion != 5);
	
	kill(pid_op1, 30);
	kill(pid_op2, 31);
	kill(pid_operador, 16);

	//Cerramos las fifos y las colas
	close(fifo_motor);
	unlink("fifo_motor");
	unlink("fifo_op2");
	unlink("fifo_operador");
	msgctl(id_cola, IPC_RMID, 0);

	//Esperamos que finalicen los procesos
	wait(&pid_op1);
	wait(&pid_op2);
	wait(&pid_operador);
	
	
	return resultado;
}
			
