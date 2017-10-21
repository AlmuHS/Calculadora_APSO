# Calculadora APSO

Esta es una calculadora programada para GNU/Linux mediante técnicas de comunicación de procesos.

La calculadora está formada por un proceso principal, llamado interfaz, que recoge los datos del usuario y los manda a tres procesos hijos a través de una cola de mensajes.
Esos datos van hacia otro proceso llamado motor, que realiza la operación, y envía el resultado a la interfaz a través de una fifo.

Funcionamiento básico de la aplicacion:
--------------------------------------

El proceso interfaz crea tres procesos: OP_1, OP_2 y OP_3; y crea una cola de mensajes entre los tres.
Ademas, crea tres fifos: fifomotor, fifo_op2 y fifo_operador, que serán usadas para comunicarse con el proceso motor.
Cuando el usuario introduce la operación a realizar, el proceso manda los datos a través de la cola para que lo lean los respectivos procesos.

El proceso OP_1 lee de la cola el primer número, crea un proceso hijo llamado motor y una pipe entre ellos, a través de la cual reenvía el primer número al motor.

El proceso OP_2 lee el segundo número de la cola, y lo reenvía al motor a través de una fifo llamada fifo_op2.

El proceso operador, lee el operador de la cola, y lo reenvía al motor a través de otra fifo llamada fifo_operador.

Finalmente, el motor lee los tres datos, efectúa la operacion, y manda el resultado a la interfaz a través de una fifo llamada fifo_motor.


Además, la aplicación incorpora las funcionalidades de operacion en cadena (activada por defecto), y de nueva operación.

Para ello, todos los procesos se ejecutan en bucle, controlados por una señal llamada fin, y con una realimentación del resultado hacia OP_1.

Al terminar una operación, el resultado se copia en la variable correspondiente al primer número, con el fin de poder ejecutar operaciones en cadena, y se desactiva la petición por teclado del primer número.

Si el usuario selecciona nueva operación, se reactiva la petición por teclado del primer número, lo que sobreescribe el resultado anterior, y da inicio a una nueva operación completa.

Cuando el usuario selecciona la opción de salir, el proceso interfaz manda la señal de fin a todos los procesos para finalizarlos.

Todo esto está controlado por la función menú, perteneciente al proceso interfaz.
