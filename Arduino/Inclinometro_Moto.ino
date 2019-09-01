

//LIBRERIAS
#include <MPU6050_tockn.h>                              //Sensor MPU6050
#include <Wire.h>                                       //Comunicaión I2c entre pantalla, sensor y arduino nano
#include <Adafruit_SSD1306.h>                           //Pantalla

//Pines
/*MPU6050
 * VCC 5v
 * GND GND
 * SDA A4
 * SCL A5
 * AD0 GND
 * INT 2
 */
 /*OLED
  * GND GND
  * VCC 5V
  * SDA A4
  * SCL A5
  */

//DECLARACIONES
#define SCREEN_WIDTH 128                        // Anchura pantalla OLED, en pixels
#define SCREEN_HEIGHT 32                        // Altura pantalla OLED, en pixels
// Declaración para la pantalla SSD1306 conectada a I2C (SDA, SCL pins)
#define OLED_RESET     4                        // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define PinPulsador  3                  //Asignamos el pin numero 3 a PinPulsador
int Pulsador = 0;                     //Creamos la variable Pulsador para que almacene el valor de PinPulsador
int AnguloX = 0;                        //Creamos la variable AnguloX para que almacene el valor del angulo en el eje X
float Temperatura = 0;                  //Creamos la variable Temperatura para que almacene el valor de del sensor de temperatura

MPU6050 mpu6050(Wire);

void setup() {
  Wire.begin();                                 //Iniciamos la comunicación Wire
  mpu6050.begin();                              //Iniciamos la comunicación con el sensor
  mpu6050.setGyroOffsets(-3.05, 1.17, -0.56);   //X : -3.05; Y : 1.17; Z : -0.56; Offsets conseguidos por serie con el prorama GetAllData
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);    // Address 0x3C for 128x32
  // Clear the buffer
  display.clearDisplay();
  // Mostrar el buffer de pantalla en la pantalla. ¡DEBES llamar a display() después de dibujar comandos para hacerlos visibles en la pantalla!
   
  display.setTextSize(3);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 3);     // Start at top-left corner
  display.print ("CBF600S");   // Texto inicial al encender la moto
  display.display();
  delay(2000);                  //Tiempo que esta mostrando el texto inicial

  pinMode(PinPulsador,INPUT);                 //Declaramos que PinPulsador va a ser entrada digital
  
}

void loop() {
  mpu6050.update();
  display.clearDisplay();

      ///////////////////Grados de inclinacion\\\\\\\\\\\\\\\\\\\\\\\\ > <
      
    AnguloX = mpu6050.getAngleX();        //Obtenemos el valor de angulo de inclinación y lo metemos en la variable AnguloX

  if(AnguloX > 90){AnguloX = 90;}         //Si la variable AnguloX es mayor que 90º lo igualamos a 90º
  if(AnguloX < -90){AnguloX = -90;}       //Si la variable AnguloX es menor que -90º lo igualamos a -90º

      //////////////////Temperatura\\\\\\\\\\\\\\\\\

    Temperatura = mpu6050.getTemp();      //Obtenemos el valor de temperatura y lo metemos en la variable Temperatura

////////////////////////////PANTALLA\\\\\\\\\\\\\\\\\\\\\\\

    Pulsador = digitalRead(PinPulsador);    //Leemos el estado del Pulsador
    if(Pulsador == HIGH){                    //Si esta en High mostramos en pantalla la inclinacion en grados
      if(AnguloX > 0 ){                      //Si el angulo es mayor a 0 colocamos una flecha en el lado derecho de la pantalla
        display.setTextSize(4);
        display.setCursor(102,3);
        display.print ((char) 16);
      }
      if(AnguloX < 0){                        //Si el angulo es menor a 0 colocamos una flecha en el lado izquierdo de la pantalla
        display.setTextSize(4);
        display.setCursor(0,3);
        display.print ((char) 17);
      }
      display.setTextSize(4);                       // Tamaño de fuente
      display.setTextColor(WHITE);                  // Draw white text
      if(AnguloX < 10 && AnguloX > -10){            // De 10 a -10 grados de inclinadion al ser un digito lo ubicamos centrado
          display.setCursor(55, 3);
      }
      else{                                           // 10 o mayor centramos los dos digitos en la pantalla
          display.setCursor(38, 3);                     // Start at top-left corner
      }
      display.print (abs(AnguloX));                      //Añadimos ",0" para que muestre el valor en la pantalla sin decimales
      // Para el simbolo de Grado "º"
      display.setTextSize(2);                       // Normal 1:1 pixel scale
      display.setTextColor(WHITE);                  // Draw white text
      //display.setCursor(110,0);                   // Start at top-left corner
      display.print("o");                           //Para mostrar el simbolo de grados "º" en la pantalla
      display.display();
    }
    else{                                           //Si esta en Low mostramos en pantalla la Temperatura ambiente
      display.clearDisplay();
      display.setTextSize(3);                       // Tamaño de fuente
      display.setTextColor(WHITE);                  // Draw white text
      display.setCursor(7, 5);                      // Start at top-left corner
      display.print(Temperatura, 1);                //Añadimos ",1" para que muestre el valor en la pantalla con un decimal
      // Para el simbolo de Grado "º"
      display.setTextSize(2);                       // Tamaño de fuente
      display.setTextColor(WHITE);                  // Draw white text
      display.setCursor(88,1);                      // Start at  top-left corner
      display.print("o");                           //Para mostrar el simbolo de grados "º" en la pantalla
      display.setTextSize(3);                       // Tamaño de fuente
      display.setCursor(105,5);                     // Start at top-left corner
      display.print("C");
      display.display();
    }
   
      
}
