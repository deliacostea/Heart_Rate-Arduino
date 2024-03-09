#include <LiquidCrystal.h>
#define USE_ARDUINO_INTERRUPTS true   
#include <PulseSensorPlayground.h>
#define red_led 13
#define yell_led 12
#define green_led 11

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);


byte heart1[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

byte heart2[8] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B01010,
  B00100,
  B00000,
  B00000
};

const int PulseWire = A0; 
const int Threshold = 550; 
PulseSensorPlayground pulseSensor; 
int Signal;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); 
  lcd.createChar(0, heart1);
  lcd.createChar(1, heart2);

  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    lcd.clear();
    lcd.print("Senzor pornit");
    delay(100);
  }
  pinMode(red_led,OUTPUT);
  pinMode(yell_led,OUTPUT);
  pinMode(green_led,OUTPUT);
}

void loop() {
  
  if (pulseSensor.sawStartOfBeat()) {
    int bpm = pulseSensor.getBeatsPerMinute();
    Serial.println(bpm);
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Ritm: ");
    lcd.print(bpm);
    lcd.setCursor(9, 0);
    lcd.write(byte(0)); 

    delay(100); 

    lcd.setCursor(9, 0); 
    lcd.write(byte(1)); 
    delay(100); 

    if(bpm >110 || bpm <50)
    {
      //  Pulsul este prea mare sau prea mic - aprinde LED-ul rosu
      digitalWrite(red_led,HIGH);
      digitalWrite(yell_led,LOW);
      digitalWrite(green_led,LOW);
    } else if ((bpm >= 100 && bpm <= 110) || (bpm >= 50 && bpm < 60)) {
      // Pulsul este în zona de avertizare - aprinde LED-ul galben
      digitalWrite(yell_led, HIGH);
      digitalWrite(red_led, LOW);
      digitalWrite(green_led, LOW);
    } else if (bpm >= 60 && bpm < 100) {
      // Pulsul este normal - aprinde LED-ul verde
      digitalWrite(green_led, HIGH);
      digitalWrite(red_led, LOW);
      digitalWrite(yell_led, LOW);
    } else {
      // În cazul în care pulsul nu este detectat stinge toate LED-urile
      digitalWrite(red_led, LOW);
      digitalWrite(yell_led, LOW);
      digitalWrite(green_led, LOW);
    }
  }
}