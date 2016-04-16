#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>

struct operacion{
	long tipo;
	int opcion;
};

int main(){
	int fifo_operador, id_cola;
	
	key_t clave_cola;
	struct operacion op;
	
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	msgrcv(id_cola, (struct msgbuf *) &op, sizeof(op) - sizeof(long), 3, 0);
	
	
	//Pasamos el operador al motor
	mkfifo("fifo_operador", 0777);
	fifo_operador=open("fifo_operador", O_WRONLY);
	write(fifo_operador, &op.opcion, sizeof(op.opcion));

	//printf("Operador: %i\n", op.opcion);
	
	return op.opcion;
}