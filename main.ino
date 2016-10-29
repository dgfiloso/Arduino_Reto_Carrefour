/*
 * NANANANANNANA
 * 
 * Sensor de stock en linea
 * autor:  Alejo
 * fecha:  29/10/2016
 * version: 1.0
 */


/**********POR MEJORAR**********
*
*  1- verificar funcion de carlcular espacios 
*  2- Mejorar condiciones de inicio(detecta objeto al empezar);  
*  3- Mejorar condiciones de finalizacion (no detecta objeto si no esta seguido de un hueco)
*/

//NOTA ---> EL CODIGO NO ESTA OPTIMIZADO, HAY MUCHAS COSAS QUE SOBRAN JEJEJEJE

#include <Ultrasonic.h>
#include <Servo.h>
/* VARIABLES*/

Servo mis;                 //servo
int angulo = 90;           //angulo de referencia

boolean f;                     //  determina si hay un onjeto o no (0->no 1 -> si)
int contH=0;  int contO=0; //  Contadores: determinan el numero de valores tomados y el cangulo girado (H-> angulo del hueco  O-> numero de datos del objeto)
boolean preF=0; boolean preFH=0;   //  almacena el valor anterior de f para determinar donde empieza un hueco u objeto y donde termina
unsigned comand;           //  valor leido del buffer
byte value=0;               //  valor leido del sensor
double sumaOb=0;           //  suma de los valores obtenidos del objeto (junto a contO se calcula el valor medio)
double lObjeto =0;         //  almacena el valor medio de la distancia



const double p = 3.1415;          // constante pi
Ultrasonic ultrasonic(2,3); // (Trig PIN,Echo PIN)


void setup() {
  mis.attach(0);          
  Serial.begin(9600);
  pinMode(4, INPUT);
}

void loop() {
  if(Serial.available()){
      comand = Serial.read();
        if(comand == 'a'){
          angulo = 180;
          Serial.flush();
          mis.write(angulo);
          for(int i =45; i<135; i++){     //Rota el angulo desde (180-i) hasta (i) 
            angulo= 180-i;
            angulo = constrain(angulo,0,180);
            mis.write(angulo);

            value = ultrasonic.Ranging(CM);
           
           if(value < 51){        //el valor maximo que da el sensor el 51, si es menos es que hay objeto
                  f = 1;
              }
              else f = 0;

           if(f == 1){            //si hay objeto, sumo los valores consecutivos que da el sensor para calcular el valor medio
              preF = f;
              sumaOb = sumaOb + (value* sin(i*p*2/360 ));
              contO++;
            }
            if(preF == 1 && f == 0 ){        //si pasa de un objeto a un hueco se finaliza la medida del objeto
                lObjeto = sumaOb/contO;
                Serial.print(" Objeto detectado a  ");  Serial.print(lObjeto);  Serial.print(" cm \n");
                sumaOb = 0;
                preF = f;
                contO = 0;
              }
              
            if (f == 0){           //calculo el angulo total girado
              preFH = 0;
              contH++;
              }
            if(preFH == 0 && f == 1){ //si pasa de un hueco a un objeto se finaliza la medida del hueco 
               unsigned ancho = 2*lObjeto*tan(contH * p /360);                    //mejorable?
               Serial.print( "Hueco de "); Serial.print(ancho); Serial.print(" cm detectado \n");
               contH=0;
               preFH =1;
              }


            delay(25);
  
        }
            mis.write(90);
        }
    }

      


}
