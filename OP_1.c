#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>


struct numero{
	long tipo;
	float num;
};


int main(){
	int pid_motor, id_cola, tubomotor[2];
	key_t clave_cola;
	struct numero num1;
	
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	msgrcv(id_cola, (struct msgbuf *) &num1, sizeof(num1) - sizeof(long), 1, 0);

	pipe(tubomotor);

	//Creamos el proceso hijo motor
	pid_motor=fork();
	if(pid_motor==0){
		close(2);
		dup(tubomotor[0]);
		execl("motor", "motor", NULL);
	}
	
	//Pasamos el primer operando al motor
	write(tubomotor[1], &num1.num, sizeof(num1.num));

	
	//printf("Primer numero: %d\n", num1.num);
	
	return num1.num;
}