/*
   NANANANANNANA

   Sensor de stock en linea
   autor:  Alejo
   fecha:  29/10/2016
   version: 1.1
*/


/**********POR MEJORAR**********

   1- verificar funcion de carlcular espacios                            ---->sospecho que es culpa de la posicion de los sensores (no estan del todo fijos)
   2- Mejorar condiciones de inicio(detecta objeto al empezar);          ----> mejorado, hacer pruenas
   3- Mejorar condiciones de finalizacion (no detecta objeto si no esta seguido de un hueco)   ---->mejorado , hcaer pruebas
*/

/********** VERSION 1.1 ****************

    cambiada la estructura para evitar condicionales y variables inecearias
    mejorarada las condiciones de inicio y de finalizacion
    mejorada la deteccion de objetos y su tamaño

*/

//NOTA ---> EL CODIGO NO ESTA OPTIMIZADO, HAY MUCHAS COSAS QUE SOBRAN JEJEJEJE

#include <Servo.h>
#include <Ultrasonic.h>


#define echoPin 2 // Echo Pin
#define trigPin 3 // Trigger Pin


/* VARIABLES*/



Servo mis;                 //servo
byte angulo = 90;           //angulo de referencia

boolean f;                     //  determina si hay un onjeto o no (0->no 1 -> si)
byte cont = 0;              //  Contadores: determinan el numero de valores tomados y el cangulo girado (H-> angulo del hueco  O-> numero de datos del objeto)
boolean preF;               //  almacena el valor anterior de f para determinar donde empieza un hueco u objeto y donde termina
unsigned comand;           //  valor leido del buffera
byte value = 0;             //  valor leido del sensor
double sumaOb = 0;         //  suma de los valores obtenidos del objeto (junto a contO se calcula el valor medio)
double lObjeto = 0;          //  almacena el valor medio de la distancia
byte angRef = 20;           //angulo inical de referencia

byte angL = angRef;       //almacena el primer angulo medido
double lValor;            //almacena el ultimo radio leido
double bValor;            //almacena el primer radio medido


const double p = 3.1415;          // constante pi

Ultrasonic ultrasonic(3,2);

void setup() {
  mis.attach(0);
  Serial.begin(9600);
//  pinMode(2, INPUT);
//  pinMode(3, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    comand = Serial.read();
    Serial.flush();
    if (comand == 'a') {
      mis.write(constrain(angRef, 0, 180));         //gira al angulo de referencia antes de empezar el bucle
      preF =  (ultrasonic.Ranging(CM) < 50);// (ultrasonic() < 50);       //el primer valor de preF es el primero que se encuenta
      delay(50);
      for (int i = angRef+1; i < 180 - angRef; i++) { //Rota el angulo desde (180-i) hasta (i)
        angulo = i;
        angulo = constrain(angulo, 0, 180);
        mis.write(angulo);
        value = ultrasonic.Ranging(CM);// ultrasonic();

        f = (value < 50);     //el valor maximo que da el sensor el 51, si es menos es que hay objeto

        if (f == 1) {          //si hay objeto, sumo los valores consecutivos que da el sensor para calcular el valor medio
          if (preF == 0) {   //si pasa de un hueco a un objeto se finaliza la medida del hueco
            cont = 0;
            angL = angulo;    //angulo en el que se empieza a detectar objeto
            bValor = ultrasonic.Ranging(CM);// ultrasonic();    //valor del radio donde se empieza a detectar el objeto
          }

          preF = f;
          sumaOb = sumaOb + (value * sin(angulo * p * 2 / 360 ));
          lValor = value;       //almacena el ultimo radio medido
          cont++;
        }


        if (f == 0) {          //calculo el angulo total girado
          if (preF == 1) {      //si pasa de un objeto a un hueco se finaliza la medida del objeto
            if (cont >= 10) {    //si el angulo barrido en el objeto es menor a 6 no es un objeto (evita falsas medidas)
              lObjeto = sumaOb / cont;
              int tamano = abs(bValor * cos(2 * p * angL / 360)) - abs(lValor * cos((angulo - 1) * 2 * p / 360));
              Serial.print("\n primer angulo "); Serial.print(angL); Serial.print(" segundo "); Serial.print(angulo); Serial.print(" distancia ");  Serial.print(lObjeto);
               Serial.print(" tamano aprox ");  Serial.println(tamano);
              sumaOb = 0;
              cont = 0;

            }
          }
          preF = 0;
          cont++;
        }
        delay(50);

      }

      mis.write(90);
    }

  }




}



