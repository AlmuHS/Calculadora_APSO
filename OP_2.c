#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>

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
	signal(32, llega_fin);
	
	do{
		msgrcv(id_cola, (struct msgbuf *) &num2, sizeof(num2) - sizeof(long), 2, 0);
		
		//Pasamos el segundo operando al motor
		fifo_op2=open("fifo_op2", O_WRONLY);
		write(fifo_op2, &num2.num, sizeof(num2.num));
	}while(fin == 0);
	
	//printf("Segundo numero: %d\n", num2.num);
	
	return num2.num;
}