#include <Servo.h>
#include "rgb_lcd.h"
#include <TimeLib.h>
#include <Time.h>
#include <Wire.h>

// Branchements
#define PIN_SERVO 7
#define PIN_BCHAT 2
#define PIN_BMENU1 3
#define PIN_BMENU2 4
#define PIN_BMENU3 5
#define PIN_BHEURE 6

// Parametres de l'ecran LCD
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_LIGS 2
#define r 255
#define g 32
#define b 192

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

// Je mets un commentaire mais je sais pas ce que c'est
int fakepoid;
int croquettesnow;
#define croquettesdepart 300
int poidcroquettes;
int decrementpoid;

int jouract;

Servo servo;
rgb_lcd lcd;

void setup()
{
    Serial.begin(9600);

    // Configuration par defaut
    dose = 50;
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
    lcd.setCursor(0, 0);
    lcd.setRGB(r, g, b);
    lcd.print("THE CAT FEEDER");
    Serial.println("THE CAT FEEDER");

    delay(2000);
    lcd.clear();
    menu();
}

void loop()
{
    if(jouract != day())
    {
        jouract = day();
        nbdistrib = 0;
        nbdemandes = 0;
        donneeauto = 0;
    }

    if(distribauto && (heureauto1 == hour() || heureauto2 == hour()) && donneeauto < 2)
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
    else if(digitalRead(PIN_BHEURE) == HIGH)
    {
        settime();
    }
}

void manger()
{
    servo.write(dose);
    servo.write(0);
}

void configurator()
{
    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        lcd.clear();
        reglages();
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        lcd.clear();
        croquettes();
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void btnconfig()
{
    int btnplus = digitalRead(digitalRead(PIN_BMENU1));

    if(btnplus == HIGH)
    {
        addconfig();
    }
}

void addconfig()
{
    maxdistribparjour = (maxdistribparjour + 1) % 5;
}

void menu()
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Menu : ");
    lcd.setCursor(8, 0);
    lcd.print("*Rgl ");
    lcd.setCursor(11, 1);
    lcd.print("1");
    lcd.setCursor(14, 0);
    lcd.print("*Crt");
    lcd.setCursor(16, 1);
    lcd.print("2");

    configurator();
}

void display()
{
    lcd.setCursor(0, 0);
    lcd.print("X Manuels : ");
    lcd.setCursor(13, 0);
    lcd.print(nbdemandes);

    lcd.setCursor(0, 1);
    lcd.print("Maximum : ");
    lcd.setCursor(11, 1);
    lcd.print(maxdistribparjour);

    btnconfig();

    delay(4500);

    menu();
}

void croquettes()
{
    lcd.setCursor(0, 0);
    lcd.print("Crts 0 : ");
    lcd.setCursor(10, 0);
    lcd.print(croquettesdepart);
    lcd.setCursor(0, 1);
    lcd.print("Crts 1 : ");
    lcd.setCursor(10, 1);
    lcd.print(croquettesnow);

    delay(4500);

    menu();
}

void reglages()
{
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
        lcd.clear();
        rglheures();
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        lcd.clear();
        display();
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void rglheures()
{
    lcd.clear();

    lcd.setCursor(0, 0);
#warning FIXME
    lcd.print("1 : ");
    lcd.setCursor(5, 0);
    lcd.print(heureauto1);
    lcd.setCursor(0, 1);
    lcd.print("2 : ");
    lcd.setCursor(5, 1);
    lcd.print(heureauto2);

    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        heureauto1 = (heureauto1 + 1) % 24;
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        heureauto2 = (heureauto2 + 1) % 24;
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void indistribcrt()
{
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("LARGUAGE EN COUR");

    for(int i = 5; i <= 10; i = i++)
    {
        lcd.setCursor(i, 1);
        lcd.print(".");
        delay(100);
    }
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

    lcd.setCursor(0, 0);

    lcd.print("HEURE");
    lcd.setCursor(7, 0);
    lcd.print("MIN");
    lcd.setCursor(4, 1);

    int H = hour(), M = minute();

    lcd.print(H);
    lcd.setCursor(5, 1);
    lcd.print(M);

    if(digitalRead(PIN_BMENU1) == HIGH)
    {
        H = (H + 1) % 24;
        setTime(H, M, second(), day(), month(), year());
    }
    else if(digitalRead(PIN_BMENU2) == HIGH)
    {
        M = (M + 1) % 60;
        setTime(H, M, second(), day(), month(), year());
    }
    else if(digitalRead(PIN_BMENU3) == HIGH)
    {
        menu();
    }
}