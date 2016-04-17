all: interfaz OP_1 OP_2 operador motor

interfaz: interfaz.c comunes.h
	gcc interfaz.c -o interfaz
	
OP_1: OP_1.c comunes.h
	gcc OP_1.c -o OP_1
	
OP_2: OP_2.c comunes.h
	gcc OP_2.c -o OP_2

operador: operador.c comunes.h
	gcc operador.c -o operador

motor: motor.c
	gcc motor.c -o motor

clean_fifo:
	rm fifo*

clean:
	rm interfaz OP_1 OP_2 operador motor