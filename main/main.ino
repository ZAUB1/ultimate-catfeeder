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

// Parametres de l'ecran LCD
#define I2C_ADDR 0x0
#define LCD_COLS 16
#define LCD_LIGS 2

#define croquettesdepart 300

// Quantite distribuee par ration
int dose;

// Rations distribuees et reclammees dans la journee et nombre maximal de rations par jour
int nbdistrib;
int nbdemandes;
int maxdistribparjour;

// Si oui ou non la distribution automatique est activee et heures de la distribution automatique
int distribauto;
int heureauto[24];

// Simulation du poids restant
int fakepoid;

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

    fakepoid = 300;
}

void loop() { //Boucle principale

    int btnmenu1 = digitalRead(portbtnmenu1); //Christian needed
    int btnmenu2 = digitalRead(portbtnmenu2); //Christian needed
    int btnretour = digitalRead(portbtnretour); //Christian needed

    int bpactiv = digitalRead(bp);

    if (heureauto == 1 && distribauto[heure])
    {
        decrementpoid = random(45, 65);

        manger();

        Serial.println("LA BOUFFE");

        heureauto = 0;
        fakepoid = fakepoid - decrementpoid;

        Serial.println("Var poid : ");
        Serial.println(fakepoid);
    }
    else if (bpactiv == 1 && distribparjour <= maxdistribparjour)
    {
        decrementpoid = random(45, 65);

        manger();

        Serial.println("Triggered distrib manuel");

        distribparjour++;
        afficheurman++;

        fakepoid = fakepoid - decrementpoid;

        Serial.println("Distribution par jour : ");
        Serial.println(distribparjour);
        Serial.println("Variable afficheur nbr de x manuel : ");
        Serial.println(afficheurman);
        Serial.println("Var poid : ");
        Serial.println(fakepoid);
    }

    croquettesnow = fakepoid;
}

void manger() { //Fonction de distribution des croquettes
    turnaround.write(valservo); // On fait tourner le servo moteur
    delay(50);
}

void configurator() {
    if (btnmenu1 == HIGH)
    {
        lcd.clear();
        reglages();
    }
    else if (btnmenu2 == HIGH)
    {
        lcd.clear();
        croquettes();
    }
    else if (btnretour == HIGH)
    {
        lcd.clear();
        menu();
    }
}

void btnconfig() {
    int btnplus = digitalRead(btnmenu1);
    
    if (btnplus == HIGH)
    {
        addconfig();
    }
}

void addconfig() {
    maxdistribparjour = (maxdistribparjour + 1) % 5;
}

void menu() {
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

void display() { //Fonction de l'afficheur
    lcd.setCursor(0, 0);
    lcd.print("X Manuels : ");
    lcd.setCursor(0, 13);
    lcd.print(afficheurman);

    lcd.setCursor(1, 0);
    lcd.print("Maximum : ");
    lcd.setCursor(1, 11);
    lcd.print(maxdistribparjour);

    delay(4500);

    menu();
}

void croquettes() {
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

void resetservo() {
    turnaround.write(0);

    Serial.println("Servo reset");
}

void reglages() {
    lcd.setCursor(0, 0);
    lcd.print("Heures");
    lcd.setCursor(1, 3);
    lcd.print("1");
    lcd.setCursor(0, 8);
    lcd.print("Manuel");
    lcd.setCursor(1, 11);
    lcd.print("2");

    if (btnmenu1 == HIGH)
    {
        lcd.clear();
        rglheures();
    }
    else if (btnmenu2 == HIGH)
    {
        lcd.clear();
        display();
    }
    else if (btnretour == HIGH)
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
