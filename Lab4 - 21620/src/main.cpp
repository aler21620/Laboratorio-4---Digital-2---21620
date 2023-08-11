//Lab 4 - 21620
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
#define resolution 8 // 1-16 bits de resolución
#define pinLedR 5
#define pinLedG 18
#define pinLedB 19
#define pinPWM 15 // GPIO 2 para tener la salida del PWM
#define buttonPin1 26 // Botón B3 para seleccionar color
#define buttonPin2 33 // Botón B4 para modificar brillo

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

//****************************************************************
// Configuración
//****************************************************************
void setup() {
  pinMode(buttonPin1, INPUT_PULLUP); // Configurar botón B3 como entrada con resistencia pull-up interna
  pinMode(buttonPin2, INPUT_PULLUP); // Configurar botón B4 como entrada con resistencia pull-up interna
  configurarPWM();

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
      colorSelected = (colorSelected + 1) % 3; // Cambiar entre Rojo, Verde y Azul
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
    case 0: // Rojo
      ledcWrite(ledRChannel, brightness);
      ledcWrite(ledGChannel, 0);
      ledcWrite(ledBChannel, 0);
      break;
    case 1: // Verde
      ledcWrite(ledRChannel, 0);
      ledcWrite(ledGChannel, brightness);
      ledcWrite(ledBChannel, 0);
      break;
    case 2: // Azul
      ledcWrite(ledRChannel, 0);
      ledcWrite(ledGChannel, 0);
      ledcWrite(ledBChannel, brightness);
      break;
  }

  // Establecer el mismo valor de brillo para todos los LEDs
  ledcWrite(pwmChannel, brightness);
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


