//Post lab 4 - 21620
//María Alejandra Rodríguez 

//****************************************************************
// Librerías
//****************************************************************
#include <Arduino.h>
#include "driver/ledc.h"

//****************************************************************
// Definición de etiquetas
//****************************************************************
#define pwmChannel 0 // 16 canales 0-15
#define ledRChannel 1
#define ledGChannel 2
#define ledBChannel 3
#define freqPWM 5000 // Frecuencia en Hz
#define freqPWMS 50   // Frecuencia en Hz (se ajusta a 50Hz para controlar el servomotor)
#define resolution 8 // 1-16 bits de resolución
#define resolutionS 10 // 1-16 bits de resolución del servo
#define pinLedR 5
#define pinLedG 18
#define pinLedB 19
#define pinPWM 15 // GPIO 2 para tener la salida del PWM
#define pinPWMS 2    // GPIO 2 para tener la salida del PWM
#define buttonPin1 26 // Botón B3 para seleccionar color
#define buttonPin2 33 // Botón B4 para modificar brillo

// Definir rangos de ángulo del servomotor
#define minimo 0   // Ángulo mínimo del servo (izquierda)
#define maximo 180 // Ángulo máximo del servo (derecha)

// Definir pines para botones
#define aumento 25
#define disminucion 32

//Posición del servo al presionar el botón 
#define servoPositionRed 135  // Posición del servo para el color rojo
#define servoPositionGreen 90  // Posición del servo para el color verde
#define servoPositionBlue 45  // Posición del servo para el color azul

volatile bool buttonPressed1 = false; //estado de los botones al inicio
volatile bool buttonPressed2 = false;

unsigned long lastDebounceTime1 = 0; //variables para los debounce (antirrebotes)
unsigned long lastDebounceTime2 = 0;

const unsigned long debounceDelay = 50;

//****************************************************************
// Prototipos de funciones
//****************************************************************
void configurarPWM(void);

//****************************************************************
// Variables Globales
//****************************************************************
int colorSelected = 0; // Variable para guardar el color seleccionado (0: Rojo, 1: Verde, 2: Azul)
int brightness = 0;    // Variable para guardar el brillo actual (0 a 255)
int inicial = 90; // Ángulo inicial del servo
//****************************************************************
// Configuración
//****************************************************************
void setup() {
  pinMode(buttonPin1, INPUT_PULLUP); // Configurar botón B3 como entrada con resistencia pull-up interna
  pinMode(buttonPin2, INPUT_PULLUP); // Configurar botón B4 como entrada con resistencia pull-up interna
  pinMode(aumento, INPUT_PULLUP); // Botón B1 habilitado con resistencia pull-up interna
  pinMode(disminucion, INPUT_PULLUP); // Botón B2 habilitado con resistencia pull-up interna
  configurarPWM();

  // Paso 1: Configurar el módulo PWM
  ledcSetup(pwmChannel, freqPWMS, resolutionS);

  // Paso 2: seleccionar en qué GPIO tendremos nuestra señal PWM
  ledcAttachPin(pinPWMS, pwmChannel);

  //Paso 3: Establecer la posición del servo motor inicialmente
  ledcWrite(0, map(180,0,180,0, 1023));

  Serial.begin(115200);
}

//****************************************************************
// Loop Principal
//****************************************************************
void loop() {
  // Leer el estado de los botones
  bool button1State = digitalRead(buttonPin1);
  bool button2State = digitalRead(buttonPin2);

  if (millis() - lastDebounceTime1 >= debounceDelay) {
    lastDebounceTime1 = millis();
    buttonPressed1 = true;
    // Si el botón B3 (buttonPin1) está presionado, cambiar el color seleccionado
    if (button1State == LOW) {
      colorSelected = (colorSelected + 1) % 4; // Cambiar entre Rojo, Verde y Azul
      delay(200); // Debounce para evitar múltiples cambios rápidos al presionar el botón
      }
  }

  if (millis() - lastDebounceTime2 >= debounceDelay) {
    lastDebounceTime2 = millis();
    buttonPressed2 = true;
    // Si el botón B4 (buttonPin2) está presionado, modificar el brillo del color seleccionado
    if (button2State == LOW) {
      brightness += 16;// Incrementar el brillo de 0 a 255
      if (brightness >= 255) {
        brightness = 0;
        }
        delay(200); // Debounce para evitar múltiples incrementos rápidos al presionar el boton
      }
  }

  // Establecer los valores de brillo para los LEDs según el color seleccionado
  switch (colorSelected) {
    case 0: // Azul
      ledcWrite(ledRChannel, 0);
      ledcWrite(ledGChannel, 0);
      ledcWrite(ledBChannel, brightness);
      ledcWrite(pwmChannel, map(servoPositionBlue, 0, 180, 30, 115));
      break;
    case 1: // Verde
      ledcWrite(ledRChannel, 0);
      ledcWrite(ledGChannel, brightness);
      ledcWrite(ledBChannel, 0);    
      ledcWrite(pwmChannel, map(servoPositionGreen, 0, 180, 30, 115));
      break;
    case 2: // Rojo
      ledcWrite(ledRChannel, brightness);
      ledcWrite(ledGChannel, 0);
      ledcWrite(ledBChannel, 0);
      ledcWrite(pwmChannel, map(servoPositionRed, 0, 180, 30, 115));
      break;
    case 3:
      ledcWrite(ledRChannel, 0);
      ledcWrite(ledGChannel, 0);
      ledcWrite(ledBChannel, 0);
      ledcWrite(pwmChannel, map(inicial, 0, 180, 30, 115)); // Mapear el ángulo a la escala del servomotor (ajustar según sea necesario)
  }

  // Establecer el mismo valor de brillo para todos los LEDs
  //ledcWrite(pwmChannel, brightness);

  if (digitalRead(aumento) == LOW){
    inicial += 5; // Aumentar el ángulo en 5 grados
    if (inicial > maximo) {
      inicial = maximo;
    }
  }

  if (digitalRead(disminucion) == LOW) {
    inicial -= 5; // Disminuir el ángulo en 5 grados
    if (inicial < minimo) {
      inicial = minimo;
    }
  }

  
  delay(100); // Añadir un pequeño retraso para evitar lecturas de botones rápidas
}

//****************************************************************
// Función para configurar módulo PWM
//****************************************************************
void configurarPWM(void) {
  // Paso 1: Configurar el módulo PWM
  ledcSetup(pwmChannel, freqPWM, resolution);
  ledcSetup(ledRChannel, freqPWM, resolution);
  ledcSetup(ledGChannel, freqPWM, resolution);
  ledcSetup(ledBChannel, freqPWM, resolution);

  // Paso 2: seleccionar en qué GPIO tendremos nuestra señal PWM
  ledcAttachPin(pinPWM, pwmChannel);
  ledcAttachPin(pinLedR, ledRChannel);
  ledcAttachPin(pinLedG, ledGChannel);
  ledcAttachPin(pinLedB, ledBChannel);
}


