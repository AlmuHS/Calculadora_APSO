#include <fcntl.h>
#include <sys/msg.h>
#include <signal.h>

#include "comunes.h"

int fin = 0;

void llega_fin(){
	fin = 1;
}

int main(){
	int fifo_operador, id_cola;
	key_t clave_cola;
	struct operacion op;
	
	//Creamos la cola
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	
	//Nos preparamos para recibir señal de fin
	signal(32, llega_fin);
	
	do{
		//Leemos el operador de la cola
		msgrcv(id_cola, (struct msgbuf *) &op, sizeof(op) - sizeof(long), 3, 0);
		
		
		//Pasamos el operador al motor
		fifo_operador=open("fifo_operador", O_WRONLY);
		write(fifo_operador, &op.opcion, sizeof(op.opcion));
	}while(fin == 0);

	//printf("Operador: %i\n", op.opcion);
	
	return op.opcion;
}