# P4_Follow_Line_SETR

Para realizar esta practica, la hemos dividido en dos principales partes, el sigue lineas y la comunicacion.

## Comunicacion

En la parte de comunicacion, lo primero que hemos implementado ha sido la comunicacion en serie. Practicamente no hemos cambiado nada de los codigos de ejemplo. Hemos aprovechado tambien para enviar un mensaje acabado en '}', como se hacia en el codigo proporcionado, para indicar a la placa arduino que la esp32 ya se ha conectado al wifi.

Para tratar los mensajes enviados por el arduino, primero los recogiamos en un buffer que detectava el final del mensaje con un caracter especial que enviavamos nosotros y otro caracter, para que en caso de que en esa accion recivida tambien trajese datos, almacenase estos en otro buffer. Esta implementacion nos fallaba en el ultimo mensaje recibido y daba muchos errores de funcionamiento a la hora de detectar que tipo de accion le llegaba en la funcion "controller_messages() ". 
Asique en una segunda implementacion, recogemos todo el string recibido hasta detectar el caracter de salto de linea '\n' en un unico buffer, y este se le proporciona a la funcion que depura este buffer. Ahora detectamos si el mensaje trae datos indicando un delimitador que nos indica si este existe o no. Si existe, separara la accion y el dato en dos strings diferentes, mientras que si solo se ha recibido  una accion no guardara nada en datos. Para que funcionen bien las comparciones de las diferentes acciones borramos el ultimo caracter del string.
Por ultimo, para implementar los mensajes JSON, lo hacemos sin usar ninguna libreria, solo detectamos cuantas de las tres opciones que puede llevar este mensaje le pasamos a la funcion encargada de crear estos mensjes y esta envia dichos mensajes con la informacion proporcionada.

## Sigue-Lineas

Para la implementacion del sigue-lineas, lo hemos hecho con arduino sketch, sin freeRTOS. 

Tambien hemos decidido hacerlo sin PID. En nuestro caso, lo controlamos con una sucesion de 'if' y 'else if'. De manera que si solo detecta la linea el sensor del centro, los motores vayan al maximo definido por la constante 'MOTOR_SPEED', si son el del centro mas uno de los lados (izquierda o  derecha), a un motor le vamos a dar 'MOTOR_SPEED', y al otro 'MOTOR_SPEED / 2', lo cual nos permite girar en caso de curva. Si solo detecla la linea uno de los sensores de los extremos (izquierda o derecha), un motor le damos 'MOTOR_SPEED', y al otro 'MOTOR_SPEED / 4' lo cual nos va a permitir girar mas que en el caso anterior. En caso de que no detectemos la linia con ningun sensor, tenemos una variable 'last', nos dice en que posicion (izquierda o  derecha), hemos visto la linea por ultima vez. Con eso, le ponemos 'MOTOR_SPEED' a un motor y el otro a '0', para que gire sobre el mismo y encuenrte la linea.

Para detectar el obstaculo, simplemente comprobamos todo el rato si el ultrasonidos detecta algo a menos de 7cm.

Tenemos dos versiones del sigue-lineas, una en modo 'slow' y la otra modo rapido. Para activar el modo slow, hay que poner a 'true' la variable 'slow', y poner 'MOTOR_SPEED' a 125.
Para poner el modo rapido, velocidad en 255 y 'slow' = false.