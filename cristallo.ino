#include <Key.h>
#include <Keypad.h>



const byte ROWS = 4; // quatre lignes 
const byte COLS = 4; // quatre colonnes
char keys [ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins [ROWS] = {5, 4, 3, 2}; // se connecter aux brochages de ligne du clavier
byte colPins [COLS] = {9, 8, 7, 6}; // se connecter aux brochages de la colonne du clavier

Keypad keypad = Keypad (makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//////////////i2c_cristal//////////////////////////
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x3F);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int dist = 13;










































;

void setup() {
Serial.begin(9600);
//////////////i2c_cristal//////////////////////////
 Wire.begin();
 Wire.beginTransmission(0x3F);
 lcd.begin(16, 2);
}

void loop() {
   char key = keypad.getKey ();
  
  if (key) {
    Serial.println(key);
  
  }
  switch (key)
    {
      case '*':
    Serial.println(key);
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Machine a cafe");
    lcd.setCursor(0, 1);
    lcd.print("Made by Donifab");
    delay(2000);
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Passez commande!");
    lcd.setCursor(0, 1);
    lcd.print("  CAFE OU THE");
    delay(2000);
    break;
    case '1':
    cafe();
    break;
     
     
    }

}

    void cafe(){
    //Serial.println(key);
    lcd.setBacklight(255);
    lcd.clear();
    digitalWrite(dist, HIGH);
    
    lcd.print("Vous avez choisi");
    lcd.setCursor(0, 1);
    lcd.print("   CAFE");
    delay(2000);
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Préparation en cours");
    lcd.setCursor(0, 1);
    lcd.print("Patientez SVP");
    delay(13000);
    digitalWrite(dist, HIGH);
    
    }

