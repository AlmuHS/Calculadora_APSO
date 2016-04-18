#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>

#include "comunes.h"

int fin = 0;

void llega_fin(){
	fin = 1;
}

int main(){
	int pid_motor, id_cola, tubomotor[2];
	key_t clave_cola;
	struct numero num1;
	
	//Abrimos la cola
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600);
	
	//Creamos la pipe
	pipe(tubomotor);
	
	//Creamos el proceso hijo motor
	pid_motor=fork();
	if(pid_motor==0){
		close(2);
		dup(tubomotor[0]);
		execl("motor", "motor", NULL);
	}
	
	//Nos preparamos para recibir señal de fin
	signal(32, llega_fin);

	
	do{
		msgrcv(id_cola, (struct msgbuf *) &num1, sizeof(num1) - sizeof(long), 1, 0);
		
		//Pasamos el primer operando al motor
		write(tubomotor[1], &num1.num, sizeof(num1.num));
	}while(fin == 0);
	
	kill(pid_motor, 9);
	
	return num1.num;
}