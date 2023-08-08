//Pre lab 4 - 21620
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
#define freqPWM 50   // Frecuencia en Hz (se ajusta a 50Hz para controlar el servomotor)
#define resolution 10 // 1-16 bits de resolución
#define pinPWM 2    // GPIO 2 para tener la salida del PWM

// Definir rangos de ángulo del servomotor
#define minimo 0   // Ángulo mínimo del servo (izquierda)
#define maximo 180 // Ángulo máximo del servo (derecha)

// Definir pines para botones
#define aumento 33
#define disminucion 26

//****************************************************************
// Variables Globales
//****************************************************************
int inicial = 90; // Ángulo inicial del servo

//****************************************************************
// Configuración
//****************************************************************
void setup() {
  pinMode(aumento, INPUT_PULLUP); // Botón B1 habilitado con resistencia pull-up interna
  pinMode(disminucion, INPUT_PULLUP); // Botón B2 habilitado con resistencia pull-up interna

  // Paso 1: Configurar el módulo PWM
  ledcSetup(pwmChannel, freqPWM, resolution);

  // Paso 2: seleccionar en qué GPIO tendremos nuestra señal PWM
  ledcAttachPin(pinPWM, pwmChannel);

  //Paso 3: Establecer la posición del servo motor inicialmente
  ledcWrite(0,map(180,0,180,0, 1023));
}

//****************************************************************
// Loop Principal
//****************************************************************
void loop() {
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

  ledcWrite(pwmChannel, map(inicial, 0, 180, 30, 115)); // Mapear el ángulo a la escala del servomotor (ajustar según sea necesario)
  delay(100); // Añadir un pequeño retraso para evitar lecturas de botones rápidas
}

