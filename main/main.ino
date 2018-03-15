#include <Servo.h>
#include "rgb_lcd.h"
#include <TimeLib.h>
#include <Time.h>
#include <Wire.h>

// Branchements
#define PIN_SERVO 12
#define PIN_BCHAT 2
#define PIN_BMENU1 3
#define PIN_BMENU2 4
#define PIN_BMENU3 5
#define PIN_BHEURE 6

// Parametres de l'ecran LCD
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_LIGS 2
int r = 255;
int g = 32;
int b = 192;
#define DD 500
#define BOUFFE 1000

// Quantite distribuee par ration
int dose;

// Rations distribuees dans la journee et nombre maximal de rations par jour
int nbdistrib;
int nbdemandes;
int maxdistribparjour;

// Si oui ou non la distribution automatique est activee et heures de la distribution automatique
int distribauto;
int heureauto1, heureauto2;
int donneeauto;

int fakepoid;
int croquettesnow;
#define croquettesdepart 300
int poidcroquettes;
int decrementpoid;

int jouract;

#define B5V 7

Servo servo;
rgb_lcd lcd;

void setup()
{
    Serial.begin(9600);

    // Configuration par defaut
    dose = 160;
    maxdistribparjour = 5;
    distribauto = 0;
    heureauto1 = 9;
    heureauto2 = 19;

    // Definition des branchements
    pinMode(PIN_BCHAT, INPUT);
    pinMode(PIN_BMENU1, INPUT);
    pinMode(PIN_BMENU2, INPUT);
    pinMode(PIN_BMENU3, INPUT);
    servo.attach(PIN_SERVO);
	
	

    // Message sur l'ecran LCD
    lcd.begin(16, 2);
    lcd.setRGB(r, g, b);
    lcd.setCursor(1, 0);
    lcd.print("THE CAT FEEDER");
    Serial.println("THE CAT FEEDER");

    for(int i = 2; i <= 13; i++)
    {
      lcd.setCursor(i, 1);
      lcd.print(".");
      delay(300);
    }

    servo.write(0);
    delay(1000);
    lcd.clear();
    menu();
}

#define P 2000
void loop()
{
  digitalWrite(B5V, 1);
  if(random(0, P) == 1)
  {
    for(int t = random(5,250); r != t; r < t ? r++ : r--)
    {
      delay(10);
      lcd.setRGB(r, g, b);}
    for(int t = random(5,250); g != t; g < t ? g++ : g--)
    {
      delay(10);
      lcd.setRGB(r, g, b);}
    for(int t = random(5,250); b != t; b < t ? b++ : b--)
    {
      delay(10);
      lcd.setRGB(r, g, b);} 
    
  }
    menu();
    if(jouract != day())
    {
        jouract = day();
        nbdistrib = 0;
        nbdemandes = 0;
        donneeauto = 0;
    }

    if((heureauto1 == hour() && donneauto == 0) || (heureauto2 == hour() && donneauto == 1))
    {
        donneeauto++;
        fakepoid -= dose;

        manger();

        Serial.println("LA BOUFFE");
        Serial.println("Var poids : ");
        Serial.println(fakepoid);
    }
    else if(digitalRead(PIN_BCHAT) == HIGH)
    {
        Serial.println("BCHAT");
        delay(DD);
        nbdemandes++;
        if(nbdistrib < maxdistribparjour)
        {
            manger();

            Serial.println("NOURRIS-MOI");

            nbdistrib++;
            fakepoid -= decrementpoid;

            Serial.print("Distributions : ");
            Serial.println(nbdistrib);
            Serial.print("Nombre de demandes : ");
            Serial.println(nbdemandes);
            Serial.print("Var poid : ");
            Serial.println(fakepoid);
        }
        else
        {
            Serial.println("Attention gruge");
        }
    }
}

void manger()
{
    servo.write(dose);
    delay(BOUFFE);
    servo.write(0);
    indistribcrt();
}

void configurator()
{
    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        Serial.println("MENU1");
        delay(DD);
        lcd.clear();
        reglages();
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        Serial.println("MENU2");
        delay(DD);
        lcd.clear();
        croquettes();
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        Serial.println("MENU3");
        delay(DD);
        lcd.clear();
        menu();
    }
    else if(digitalRead(PIN_BHEURE) == HIGH)
    {
        Serial.println("BHEURE");
        delay(DD);
        lcd.clear();
        settime();
    }
}

void btnconfig()
{
    int btnplus = digitalRead(digitalRead(PIN_BMENU1));

    if(btnplus == HIGH)
    {
        Serial.println("MENU1");
        delay(DD);
        addconfig();
    }
}

void addconfig()
{
    maxdistribparjour = (maxdistribparjour + 1) % 5;
}

void menu()
{
    lcd.setCursor(0, 0);
    lcd.print("*Rgl ");
    lcd.setCursor(1, 1);
    lcd.print("1");
    lcd.setCursor(7, 0);
    lcd.print("*Crt");
    lcd.setCursor(8, 1);
    lcd.print("2");

    configurator();
}

void display()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X Manuels : ");
    lcd.setCursor(13, 0);
    lcd.print(nbdemandes);

    lcd.setCursor(0, 1);
    lcd.print("Maximum : ");
    lcd.setCursor(11, 1);
    lcd.print(maxdistribparjour);


    delay(4500);
    lcd.clear();

}

void croquettes()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Crts 0 : ");
    lcd.setCursor(10, 0);
    lcd.print(croquettesdepart);
    lcd.setCursor(0, 1);
    lcd.print("Crts 1 : ");
    lcd.setCursor(10, 1);
    lcd.print(croquettesnow);

    delay(4500);
    lcd.clear();
}

void reglages()
{
    lcd.clear();
  rgl:
    lcd.setCursor(0, 0);
    lcd.print("Heures");
    lcd.setCursor(3, 1);
    lcd.print("1");
    lcd.setCursor(8, 0);
    lcd.print("Manuel");
    lcd.setCursor(11, 1);
    lcd.print("2");

    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        Serial.println("MENU1");
        delay(DD);
        lcd.clear();
        rglheures();
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        Serial.println("MENU2");
        delay(DD);
        lcd.clear();
        display();
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        Serial.println("MENU3");
        delay(DD);
        lcd.clear();
        return;
    }
goto rgl;
}

void rglheures()
{
    lcd.clear();

rglh:
    lcd.setCursor(0, 0);
    lcd.print("1 : ");
    lcd.setCursor(5, 0);
    lcd.print(heureauto1);
    lcd.setCursor(0, 1);
    lcd.print("2 : ");
    lcd.setCursor(5, 1);
    lcd.print(heureauto2);

    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        Serial.println("MENU1");
        delay(DD);
        heureauto1 = (heureauto1 + 1) % 24;
    lcd.clear();
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        Serial.println("MENU2");
        delay(DD);
        heureauto2 = (heureauto2 + 1) % 24;
    lcd.clear();
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        Serial.println("MENU3");
        delay(DD);
        lcd.clear();
        return;
    }
    goto rglh;
}

void indistribcrt()
{
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("LARGUAGE EN COUR");

    for(int i = 5; i <= 10; i++)
    {
        lcd.setCursor(i, 1);
        lcd.print(".");
        delay(200);
    }
    
    lcd.clear();
}

void displayafterfood()
{
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("AVANT / APRES");
    lcd.setCursor(3, 1);
    lcd.print(croquettesdepart);
    lcd.setCursor(11, 1);
    lcd.print("-");
    lcd.setCursor(13, 1);
    lcd.print(decrementpoid);
}

void settime()
{
    lcd.clear();
 sett:

    lcd.setCursor(0, 0);

    lcd.print("HEURE");
    lcd.setCursor(7, 0);
    lcd.print("MIN");
    lcd.setCursor(2, 1);

    int H = hour(), M = minute();

    lcd.print(H);
    lcd.setCursor(8, 1);
    lcd.print(M);

    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        Serial.println("MENU1");
        delay(DD);
        H = (H + 1) % 24;
        setTime(H, M, second(), day(), month(), year());
    lcd.clear();
    goto sett;
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        Serial.println("MENU2");
        delay(DD);
        M = (M + 1) % 60;
        setTime(H, M, second(), day(), month(), year());
    lcd.clear();
    goto sett;
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        Serial.println("MENU3");
        delay(DD);
    lcd.clear();
        menu();
        return;
    }
    goto sett;
}
