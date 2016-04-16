#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>

struct numero{
	long tipo;
	float num;
};

int main(){
	int fifo_op2, id_cola;
	key_t clave_cola;
	struct numero num2;
	
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	msgrcv(id_cola, (struct msgbuf *) &num2, sizeof(num2) - sizeof(long), 2, 0);
	
	//Pasamos el segundo operando al motor
	mkfifo("fifo_op2", 0777);
	fifo_op2=open("fifo_op2", O_WRONLY);
	write(fifo_op2, &num2.num, sizeof(num2.num));

	//printf("Segundo numero: %d\n", num2.num);
	
	return num2.num;
}