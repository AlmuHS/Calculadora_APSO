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

struct operacion{
	long tipo;
	int opcion;
};


void menu(float *num1, float *num2, int *operacion){
	printf("Introduce un numero: ");
	scanf("%f", num1);
	
	printf("Operacion\n");
		printf("1. Suma\n");
		printf("2. Resta\n");
		printf("3. Multiplicacion\n");
		printf("4. Division\n");
	do{
		printf("Selecciona operacion: ");
		scanf("%d", operacion);
	}while(*operacion < 1 || *operacion > 4);
	do{
		printf("Introduce otro numero: ");
		scanf("%f", num2);
	}while(*num2 == 0);
}

int main(){
	float resultado; 
	int pid_op1, pid_op2, pid_operador;
	int fifo_motor;
	key_t clave_cola;
	int id_cola;
	
	struct operacion op;
	struct numero num1, num2;
	
	//Creamos la cola para pasar los parametros
	clave_cola=ftok("./Makefile", 1024);
	id_cola=msgget(clave_cola, 0600 | IPC_CREAT);
	
	//Obtenemos los parametros
	menu(&num1.num, &num2.num, &op.opcion);
	
	//Indicamos el tipo de los parametros
	op.tipo=3;
	num1.tipo=1;
	num2.tipo=2;
	
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
	
	//Pasamos el primer operando al proceso OP_1
	msgsnd(id_cola, (struct msgbuf *) &num1, sizeof(num1) - sizeof(long), 0);
	
	//Pasamos el operador al proceso operador
	msgsnd(id_cola, (struct msgbuf *) &op, sizeof(op) - sizeof(long), 0);

	//Pasamos el segundo operando al proceso OP_2
	msgsnd(id_cola, (struct msgbuf *) &num2, sizeof(num2) - sizeof(long), 0);
	
	
	//Leemos el resultado del motor
	fifo_motor=open("fifo_motor", O_RDWR);
	read(fifo_motor, &resultado, sizeof(resultado));
	
	printf("Resultado: %f\n", resultado);
	
	close(fifo_motor);
	unlink("fifo_motor");
	
	msgctl(id_cola, IPC_RMID, 0);
	
	return resultado;
}
			
