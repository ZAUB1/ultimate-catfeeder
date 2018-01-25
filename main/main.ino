#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Time.h>
#include <Wire.h>

// Branchements
#define PIN_SERVO 0
#define PIN_BCHAT 0
#define PIN_BMENU1 0
#define PIN_BMENU2 0
#define PIN_BMENU3 0
#define PIN_BHEURE 0

// Parametres de l'ecran LCD
#define LCD_ADDR 0x0
#define LCD_COLS 16
#define LCD_LIGS 2

// Quantite distribuee par ration
int dose;

// Rations distribuees dans la journee et nombre maximal de rations par jour
int nbdistrib;
int nbdemandes;
int maxdistribparjour;

// Si oui ou non la distribution automatique est activee et heures de la distribution automatique
int distribauto;
int heureauto[24];

// Je mets un commentaire mais je sais pas ce que c'est
int fakepoid;
int croquettesnow;
#define croquettesdepart 300
int poidcroquettes;
int decrementpoid;

int jouract;

Servo servo;
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_LIGS);

void setup()
{
    Serial.begin(9600);

    // Configuration par defaut
    dose = 50;
    maxdistribparjour = 5;
    distribauto = 0;
    heureauto[9] = 1;
    heureauto[19] = 1;

    // Definition des branchements
    pinMode(PIN_BCHAT, INPUT);
    pinMode(PIN_BMENU1, INPUT);
    pinMode(PIN_BMENU2, INPUT);
    pinMode(PIN_BMENU3, INPUT);
    servo.attach(PIN_SERVO);

    // Message sur l'ecran LCD
    lcd.init();
    lcd.setCursor(0, 0);
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

        for(int i = 0; i < 24; i++)
        {
            distribdonnee[i] = 0;
        }
    }

    if(heureauto && distribauto[hour()] && !distribdonnee[hour()])
    {
        distribdonnee[hour()] = 1;
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

            distribparjour++;
            fakepoid -= decrementpoid;

            Serial.printt("Distributions : ");
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
    else if(digitalRead(BHEURE) == HIGH)
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
    if(btnmenu1 == HIGH)
    {
        lcd.clear();
        reglages();
    }
    else if(btnmenu2 == HIGH)
    {
        lcd.clear();
        croquettes();
    }
    else if(btnretour == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void btnconfig()
{
    int btnplus = digitalRead(btnmenu1);

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
    lcd.setCursor(0, 8);
    lcd.print("*Rgl ");
    lcd.setCursor(1, 11);
    lcd.print("1");
    lcd.setCursor(0, 14);
    lcd.print("*Crt");
    lcd.setCursor(1, 16);
    lcd.print("2");

    configurator();
}

void display()
{
    lcd.setCursor(0, 0);
    lcd.print("X Manuels : ");
    lcd.setCursor(0, 13);
    lcd.print(afficheurman);

    lcd.setCursor(1, 0);
    lcd.print("Maximum : ");
    lcd.setCursor(1, 11);
    lcd.print(maxdistribparjour);

    btnconfig();

    delay(4500);

    menu();
}

void croquettes()
{
    lcd.setCursor(0, 0);
    lcd.print("Crts 0 : ");
    lcd.setCursor(0, 10);
    lcd.print(croquettesdepart);
    lcd.setCursor(1, 0);
    lcd.print("Crts 1 : ");
    lcd.setCursor(1, 10);
    lcd.print(croquettesnow);

    delay(4500);

    menu();
}

void resetservo()
{
    turnaround.write(0);

    Serial.println("Servo reset");
}

void reglages()
{
    lcd.setCursor(0, 0);
    lcd.print("Heures");
    lcd.setCursor(1, 3);
    lcd.print("1");
    lcd.setCursor(0, 8);
    lcd.print("Manuel");
    lcd.setCursor(1, 11);
    lcd.print("2");

    if(btnmenu1 == HIGH)
    {
        lcd.clear();
        rglheures();
    }
    else if(btnmenu2 == HIGH)
    {
        lcd.clear();
        display();
    }
    else if(btnretour == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void rglheures() {
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("1 : ");
    lcd.setCursor(0, 5);
    lcd.print(heureauto1);
    lcd.setCursor(1, 0);
    lcd.print("2 : ");
    lcd.setCursor(1, 5);
    lcd.print(heureauto2);

    if (btnmenu1 == HIGH)
    {
        heureauto1 = (heureauto1 + 1) % 24;
    }
    else if (btnmenu2 == HIGH)
    {
        heureauto2 = (heureauto2 + 1) % 24;
    }
    else if (btnretour == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void indistribcrt() {
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("LARGUAGE EN COUR");

    for (i = 5; i <= 10; i = i++)
    {
        lcd.setCursor(1, i);
        lcd.print(".");
        delay(100);
    }
}

void displayafterfood() {
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("AVANT / APRES");
    lcd.setCursor(1, 3);
    lcd.print(croquettesdepart);
    lcd.setCursor(1, 11);
    lcd.print("-");
    lcd.setCursor(1, 13);
    lcd.print(decrementpoid);
}

void rsttime() {
    if (heure == 23)
    {
        distribparjour = 0;
        afficheurman = 0;
    }
}

void settime() {
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("HEURE");
    lcd.setCursor(0, 7);
    lcd.print("MIN");
    lcd.setCursor(1, 4);
    lcd.print(hour);
    lcd.setCursor(1, 5);
    lcd.print(min);

    if (btnmenu1 == HIGH)
    {
        hour++;
    }
    else if (btnmenu2 == HIGH)
    {
        min++;
    }
    else if (btnretour == HIGH)
    {
        menu();
    }
}
