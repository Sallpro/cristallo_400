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
byte rowPins [ROWS] = {3, 2, 1, 0}; // se connecter aux brochages de ligne du clavier
byte colPins [COLS] = {7, 6, 5, 4}; // se connecter aux brochages de la colonne du clavier
Keypad keypad = Keypad (makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//////////////i2c_cristal//////////////////////////
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x3F);  // attribution de l'adresse du module i2c
//declaration des variables globales
int dist =12; //pin du relais controlant pour le lait (le moteur qui pousse le soluble, le moteur qui fait melange, et le verou pour l'eau)
int Eau= 11;//pin du relais controlant la pompe à eau pour tout les solubles(220V).
int dist_suc=10;//pin du relais controlant le moteur qui pousse le sucre
int eaucafe= 9;//pin du relais controlant le verrou pour l'eau, pour le café
int distcafe= 8;// pin du relais controlant pour le café (le moteur qui pousse le soluble, le moteur qui fait melange,)

int etatMachine = LOW;// initialisation de l'état de la machine à LOW, éteinte 

/*cette partie du code est optionnelle dans mon cas, car elle controle la pose automatique du gobelet qui nous avons ignoré pour pouvoir utiliser
 * d'autre tasses.Mais au cas où voulez utilisez cette option, alors vous aurez besoin de commannder le bras de livraison des boisson et le bras de versement 
 * du soucre. les code concernant cette partie seront sous forme de commentaire.
 
int pose_gobelet= //pin du relais controlant le moteur de pose de gobelet(220V)
int brasboisson= // pin du relais controlant le moteur du bras de livraison de boisson
int brassucre = //pin du relais controlant le moteur du bras de versement du sucre

int positionbrasboisson = //pin pour la lecture d'un bouton poussoir qui indique que le bras est en bonne position
int positionbrassucre = //pin pour la lecture d'un bouton poussoir qui indique que le bras est en bonne position
int Etatbrasboisson = 0; 
int Etatbrassucre = 0; 
*/


////////////////////////////////////////SETUP///////////////////////////////////////////////////////
void setup() {
Serial.begin(9600);//ouverture de la cummunication serie
//////////////i2c_cristal//////////////////////////
 Wire.begin();
 Wire.beginTransmission(0x3F);
 lcd.begin(16, 2);
 pinMode(dist,OUTPUT);
 pinMode(Eau,OUTPUT);
 pinMode(dist_suc,OUTPUT);
 pinMode(distcafe,OUTPUT);
 pinMode(eaucafe,OUTPUT);

 
/* pinMode(pose_gobelet,OUTPUT);
   pinMode(brasboisson,OUTPUT);
   pinMode(brassucre,OUTPUT);
   pinMode(positionbrasboisson,INPUT);
   pinMode(positionbrassucre,INPUT);
 * 
 */

 
}
/////////////////////////////////////////LOOP///////////////////////////////////////////////////////
void loop() {
 //par defaut l'ecran de la machine est etteinte
  
   char key = keypad.getKey (); 
    if (key){
    Serial.println(key);
  } 
   if (key == '*') //une fois que la touche * est appuyée, 
   {
   Serial.println(key);
   
   etatMachine = !etatMachine; //l'etat de l amachine change, (de etteinte à allumée et vice versa) 
  }
  if (etatMachine !=LOW) // si la machine est allumée
  {

   main:// création d'un repère main pour indentifier notre point de depart
   
    Serial.println("ON");
//on affiche ce message sur l'ecran une fois que la machine est allumée
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Passez Commande");
    lcd.setCursor(0, 1);
    lcd.print("CAFE THE LAIT");
    delay(10);
    
    switch (key)// ensuite si la touche 1 qui correspond ici au lait 
    {
      case '1':// boutton pour le lait
  //on affiche ce message sur l'ecran
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Preparation");
    lcd.setCursor(0, 1);
    lcd.print("Patientez SVP");
    delay(10);
        
 distsuc(); // on appelle la fonction de distribution du sucre
 
 delay(5000);
 
 preparation();//on appelle la fonction de préparation
 
 delay(5000);
  // quand c'est prêt alors on affiche ceci
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("   MERCI");
    lcd.setCursor(0, 1);
    lcd.print(" AU REVOIR");
    delay(5000);
 break; // on met fin à l'action declenchée par le boutton 1
 goto main; // et on retourne à notre repère main
//////////////////////////////////////////////////café/////////////////////////////////// 

   case '3':// la touche 3 correspond au café, et la même procedure

    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Preparation");
    lcd.setCursor(0, 1);
    lcd.print("Patientez SVP");
    delay(10);
        
 distsuc();
 
 delay(5000);
 
 cafe();
 
 delay(5000);
 
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("   MERCI");
    lcd.setCursor(0, 1);
    lcd.print(" AU REVOIR");
    delay(5000);
 break;
 goto main;
 ///////////////////////////////////////café au lait///////////////////////////////
 
 case '2':// la touche 2 correspond au café au lait et toujours la même procedure
  
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("Preparation");
    lcd.setCursor(0, 1);
    lcd.print("Patientez SVP");
    delay(10);
        
 distsuc();
 
 delay(5000);
 
 cafeaulait();
 
 delay(5000);
 
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("   MERCI");
    lcd.setCursor(0, 1);
    lcd.print(" AU REVOIR");
    delay(5000);
 break;
 goto main;
   
    }

  }
///////c'est cette instruction qui permet de maintenir l'ecran etteinte  
  else{
   Serial.println("off");
    lcd.setBacklight(0);
    lcd.clear();
  }
}


///////////////////////////////////////////////////function////////////////////



void distsuc()// la fonction de distribution du sucre on allume le moteur pendant 4 seconde dans mon cas, le bras ne bouge pas car il y a pas de pose automatique de gobelet
// s'il y avait pose auto de gobelet, il aurait fallu  replier le bras pour laisser passer le gobelet avant. et le code serait different, avec lecture de l'etat du boutton.
{
  digitalWrite(dist_suc,HIGH);
  delay(4000);
  digitalWrite(dist_suc,LOW);
}

void preparation()
{
  digitalWrite(dist,HIGH);
  delay(5000);
  digitalWrite(Eau,HIGH);
  delay(15000);
  digitalWrite(Eau,LOW);
  digitalWrite(dist,LOW);
}
void cafe(){
  

  digitalWrite(Eau,HIGH);
  digitalWrite(eaucafe,HIGH);
  digitalWrite(distcafe,HIGH);
  delay(5000);
  digitalWrite(distcafe,LOW);
  delay(10000);
  digitalWrite(Eau,LOW);
  digitalWrite(eaucafe,LOW);
}
void cafeaulait(){
  digitalWrite(distcafe,HIGH);
  delay(5000);
  digitalWrite(distcafe,LOW);
  digitalWrite(Eau,HIGH);
  digitalWrite(eaucafe,HIGH);
  delay(8000);
  digitalWrite(Eau,LOW);
  digitalWrite(eaucafe,LOW);
  
  delay(5000);
  digitalWrite(dist,HIGH);
  delay(5000);
  digitalWrite(Eau,HIGH);
  delay(8000);
  digitalWrite(Eau,LOW);
  digitalWrite(dist,LOW);  
}

