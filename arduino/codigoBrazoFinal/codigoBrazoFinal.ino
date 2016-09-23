/* 
 * TÍTULO :  Código para el brazo para la prueba 5 + 1
 *
 * AUTOR  :  Jesús Cruz Olivera - Centro de Control 
 *
 * NOTAS  :  utilizar mavlink como protocolo de comunicación, un mavlink más pequeño
 *           posible tfg: protocolo mavlink exclusivamente para brazos
 *
 *           Al usar el protocolo serial tenemos problemas para enviar los datos
 *           sin funciones para empaquetar, desempaquetar, cálculo de CRCs...
 *           Para el 5 + 1 no hay problema ya que solo hay un posible mensaje
 *           pero si se quiere ampliar habría que añadir la librería mavlink
 *           con nuevos mensajes específicos para el brazo
 *
 *           Para subir el código al arduino no deberá estar conectada la px4 
 *           por el puerto serial, para desconectar el serial desconectar los 
 *           jumpers gris y blanco, una vez subido el código volver a conectarlos
 */

#include <Servo.h>                   // Clase Servo        

Servo superior;
Servo medio;
Servo inferior;
Servo amarre;

long mensaje;                       //Mensaje recibido por el serial

//Ángulos calculados para los movimientos, si cambian modificarlos aquí:
const int pinzaAbrir = 60;          // Ángulo de apertura de la pinza.
const int pinzaCerrar = 40;          
const int desplegarSuperior = 80;   // Ángulo para el servo superior al desplegar.
const int desplegarInferior = 125;
const int recogerSuperior = 60; 
const int recogerInferior = 20;
const int fijoMedio = 60;           // Ángulo para el servo medio, es fijo.

//Ángulo máximo de apertura para los motores, si cambian modificarlos aquí
const int maxServoAngle = 180;

/* Abrimos el serial, indicamos dónde están conectados los servos y cojemos 
 * el paquete */
void setup()
{
    Serial.begin(9600);     
    
    superior.attach(6);             //servo superior pin 6 base
    medio.attach(10);               //servo brazo pin 10 
    inferior.attach(5);             //servo inferior pin 5 
    amarre.attach(11);              //servo amarre pin 11 pinza

    
    medio.write(60);                //este motor no se usa en los movimientos actuales
    // Debemos escribir unos valores iniciales para evitar posibles errores al usar servo.read();
    inferior.write(20);
    superior.write(60);
    amarre.write(60);
    cogerPaqueteSuelo();    
}

/* Comprobamos el puerto serial hasta que nos llega el mensaje esperado */
void loop() 
{
  // moverBrazo(); para probar sin el serial, útil para demostraciones sin la px4
    if (Serial.available() > 0) {
        mensaje = Serial.read();
        if ( mensaje == 8 || mensaje == 7 ){    // revisar notas,en cabecera 
            //Serial.print(mensaje);
            moverBrazo();                       // movemos el brazo
        }
    }
}


/* Función principal con los movimientos del brazo a realizar para entrega */
void moverBrazo()
{
    desplegar();
    delay(1000);

    abrirPinza();
    delay(1000);
    cerrarPinza();

    recoger();
    delay(1000);
}


/* Recogemos el brazo y abrimos la pinza para introducir el paquete */
void cogerPaqueteSuelo()
{
    recoger();
    abrirPinza();
    delay(2000);
    
    cerrarPinza();
    delay(2000);
}

/*==============================Despliegue brazo==============================*/
/* Comienza a bajar el brazo. */
void desplegar()
{
    writeAngleAndTime( superior, desplegarSuperior, 2000 );
    writeAngleAndTime( inferior, desplegarInferior, 2000 );
}


/*==============================Soltar Paquete================================*/
/* Soltar paquete girando el servo 50 grados y asi se abre la pinza. */
void abrirPinza()
{
    writeAngleAndTime( amarre, pinzaAbrir , 500 );
}


/*==============================Agarrar Paquete===============================*/
/* se cierra la pinza en esta posición para de esta forma poder amarrar un 
 * paquete en la posición de recogido.
 */
void cerrarPinza()
{
    writeAngleAndTime( amarre, pinzaCerrar, 500 );
}


/*================================Recoger brazo===============================*/
/* Recoger brazo
 * La forma es exactamente igual que la explicada para bajar el brazo pero de 
 * manera inversa, el valor superior varía de 80 a 30 y los valores medio e 
 * inferior cambian a partir del anterior con una función.
 */
void recoger()
{
    writeAngleAndTime( inferior, recogerInferior, 1500 );
    writeAngleAndTime( superior, recogerSuperior, 2000 );  
}


/*===============================Recoger brazo================================*/
/* Mover un servo hasta el ángulo dado tardando el tiempo dado en milisegundos.
 *
 * TODO: usar módulos en vez de restas y ahorrar código, constantes, booleanos,
 * milisegundos, protothreads, valor de retorno ... 
 */
bool writeAngleAndTime( Servo servo, int angle, int time )
{
    int actualAngle = servo.read();     
    int wait = time/angle;          // El tiempo de espera tras cada movimiento 
    int timeSpent = wait * angle;   // El tiempo aproximado que tardará 
    
    if ( actualAngle < angle ) {
      for ( int i = actualAngle ; i <= angle ; i ++){
        servo.write(i);
        delay(wait);
      }
    }
    else {
      wait = ( time/(maxServoAngle-0) );
      timeSpent = wait * ( maxServoAngle - angle ) ;
      for ( int i = actualAngle ; i >= angle ; i --){
        servo.write(i);
        delay(wait);
      }
    } 
    
    // Si hemos tardado menos tiempo del deseado esperamos lo que falte
    if ( timeSpent != time ){       
        servo.write(angle);
        delay(time-timeSpent);
    }
    return true;
}
