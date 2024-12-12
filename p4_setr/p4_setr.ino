#include "PID_Controller.h"
#include "FastLED.h"
#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];


// Ultrasonics
#define TRIG_PIN 13  
#define ECHO_PIN 12  

// Infrarrojos
#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

// Enable/Disable motor control.
//  HIGH: motor control enabled
//  LOW: motor control disabled
#define PIN_Motor_STBY 3

// Group A Motors (Right Side)
// PIN_Motor_AIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_AIN_1 7
// PIN_Motor_PWMA: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMA 5

// Group B Motors (Left Side)
// PIN_Motor_BIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_BIN_1 8
// PIN_Motor_PWMB: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMB 6


#define MOTOR_SPEED 200

#define RIGHT 0
#define LEFT 1

# define SENS_THRESHOLD 650

int last = RIGHT;
bool ready = false;

void setup() {
  // UltraSonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // Infrarrojos
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

  // Motores
  // enable
  pinMode(PIN_Motor_STBY, OUTPUT);
  // derecha
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  // izq
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);

  Serial.begin(9600);

  digitalWrite(PIN_Motor_STBY, 1);
  digitalWrite(PIN_Motor_AIN_1, 1);
  digitalWrite(PIN_Motor_BIN_1, 1);

  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

}

void loop() {
  int error;
  bool sl = false, sc = false, sr = false;
  // PID_Controller *pid = new PID_Controller(127, 0, 0, 0);
  // put your main code here, to run repeatedly:
  // analogWrite(PIN_Motor_PWMA, 0);
  // analogWrite(PIN_Motor_PWMB, 0);

  int left = analogRead(PIN_ITR20001_LEFT);
  int center = analogRead(PIN_ITR20001_MIDDLE);
  int right = analogRead(PIN_ITR20001_RIGHT);

  if (left > SENS_THRESHOLD){ sl = true; }
  if (center > SENS_THRESHOLD){ sc = true; }
  if (right > SENS_THRESHOLD){ sr = true; }

  if (last == RIGHT){
    FastLED.showColor(Color(255, 0, 255));
  } else {
    FastLED.showColor(Color(0, 255, 0));
  } 

  if (ready){
    if (sc && !sl && !sr){
      Serial.println("cent: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED);
    } else if (sc && sl && !sr) {
      Serial.println("center left: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED / 2);
      last = LEFT;
    } else if (sc && !sl && sr) {
      Serial.println("center right: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED / 2);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED);
      last = RIGHT;
    } else if (sc && !sl && sr) {
      Serial.println("left: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED / 4);
      last = LEFT;
    } else if (!sc && !sl && sr) {
      Serial.print("right: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED / 4);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED);
      last = RIGHT;
    } else if (!sc && !sl && !sr && last == LEFT) {
      Serial.print("none left: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED / 4);
      // analogWrite(PIN_Motor_PWMA, MOTOR_SPEED / 4);
      // analogWrite(PIN_Motor_PWMB, 0);
    } else if (!sc && !sl && !sr && last == RIGHT) {
      Serial.print("none right: ");
      analogWrite(PIN_Motor_PWMA, MOTOR_SPEED / 4);
      analogWrite(PIN_Motor_PWMB, MOTOR_SPEED);
      // analogWrite(PIN_Motor_PWMA, 0);
      // analogWrite(PIN_Motor_PWMB, MOTOR_SPEED / 4);
    }

    if (dist() < 10) {
      ready = false;
      analogWrite(PIN_Motor_PWMA, 0);
      analogWrite(PIN_Motor_PWMB, 0);
    }
  } else {
    float d = dist();
    Serial.println(d);
    if (d < 5) {
      ready = true;
      delay(2000);
    }
  }
}

float dist(){
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(TRIG_PIN, LOW);
  
  t = pulseIn(ECHO_PIN, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm

  return d;
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

