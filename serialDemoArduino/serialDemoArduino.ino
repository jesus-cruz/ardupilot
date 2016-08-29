/* 
 * Sencillo programa para demostrar que el arduino recibe datos por el puerto serie
 * 
 * Autor: Jesús Cruz Olivera - Centro de Control - Drotium
 * 
 * 1 - Podemos probarlo usando el serial monitor del IDE de arduino, el led integrado debería
 * parpadear al recibir datos por el serial. 
 * 2 - Una vez funcione de la anterior manera probarlo con algún programa en C y un sistema linux
 * 3 - Por último conectar la pixhawk y probar a enviar datos
 * 
 * Cuando consigamos verificar que recibimos información deberíamos pasar a comprobar dicha información
 * 
 */
 
void setup() {
        Serial.begin(9600);     // serial port a 9600 bps
        pinMode(13, OUTPUT);    // usamos el led integrado
}

void loop() {

        // Si tenemos datos disponibles en el serial 
        if (Serial.available() > 0) {
               digitalWrite(13, HIGH);  // on
               delay(1000);
               Serial.read();
        }
        digitalWrite(13, LOW);          // off
        delay(1000);
}

