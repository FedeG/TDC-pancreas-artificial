#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RGB_BLUE 2
#define RGB_GREEN 4
#define RGB_RED 6

enum Color {RED, GREEN, BLUE};
enum State {NORMAL, WARNING, WAITING, INJECTING};

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

const int waterSensorPin = A0;

unsigned long lastMeasurementTime = 0;
unsigned long prevMeasurementTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(waterSensorPin, INPUT);

  lcd.init();
  lcd.backlight();

  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
}

int getGlucemia(){
  int waterLevel = analogRead(waterSensorPin);
  lastMeasurementTime = millis();
  return map(waterLevel, 0, 1023, 50, 300); // +40-60
}

void showGlucemia(int glucemia) {
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at line 0, char 0
  lcd.print("TDC: Glucemia");
  lcd.setCursor(0,1); // go to start of 2nd line
  lcd.print(glucemia);
  lcd.setCursor(11,1); // go to start of 2nd line // mg/dl
  lcd.print("mg/dl");
}

void setLed(Color color){
  switch (color)
  {
    case GREEN:
      digitalWrite(RGB_GREEN, HIGH);
      digitalWrite(RGB_BLUE, LOW);
      digitalWrite(RGB_RED, LOW);
      break;
    case RED:
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, LOW);
      digitalWrite(RGB_RED, HIGH);
      break;
    case BLUE:
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, HIGH);
      digitalWrite(RGB_RED, LOW);
      break;
    default: break;
  }
}

int getDelayByState(State state){
  switch (state)
  {
    case WARNING: return 500;
    case NORMAL: return 1000;
    case INJECTING: return 2000;
    case WAITING: return 15000;
    default: return 500;
  }
}

float ValorRef = 180;
float f = 0;
float e = ValorRef - f;
float ePrev;

int getDelayByValue(){
  if (e < 50 and e >= 0) {
    return getDelayByState(WARNING);
  }

  return getDelayByState(NORMAL);
}

void showValues(float dedt, boolean P, boolean D) {
  Serial.print("Valores t: ");
  Serial.print(lastMeasurementTime);
  Serial.print(" Vref: ");
  Serial.print(ValorRef);
  Serial.print(" f: ");
  Serial.print(f);
  Serial.print(" e: ");
  Serial.print(e);
  Serial.print(" de/dt: ");
  Serial.print(dedt);
  Serial.print(" P: ");
  Serial.print(P);
  Serial.print(" D: ");
  Serial.print(D);
  Serial.print(" PD: ");
  Serial.print(P or D);
  Serial.println("");
}

float getDerivativeValue() {
  float de = (e - ePrev);
  float dt = (lastMeasurementTime - prevMeasurementTime) / 1000.0;

  return de/dt;
}

void loop() {
  f = getGlucemia();
  showGlucemia(f);

  // Punto suma
  e = ValorRef - f;

  float dedt = getDerivativeValue();
  boolean P = e < 0;
  boolean D = (ePrev and getDerivativeValue() < -30);
  showValues(dedt, P, D);

  if (P or D) {
    Serial.println("Inyectando 1 unidad internacional de insulina");
    setLed(RED);
    delay(getDelayByState(INJECTING));
    Serial.println("Esperando metabolización de la insulina");
    setLed(BLUE);
    delay(getDelayByState(WAITING));
  } else {
    setLed(GREEN);
    delay(getDelayByValue());
  }

  ePrev = e;
  prevMeasurementTime = lastMeasurementTime;
}
