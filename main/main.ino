#include <Time.h> // Synchro tps avec l'ordi
#include <LiquidCrystal.h> // Lib afficheur liquid
#include <Servo.h> // Lib servo
#include <Wire.h>

//Config
int distribparjour;
int maxdistribparjour = 0;
#define valservo 42 // A chercher

//Port afficheur liquid
#define portafficheurRS 42 // Need christian
#define portafficheur2 42  // Need christian
#define portafficheur3  42 // Need christian
#define portafficheur4 42  // Need christian
#define portafficheur5  42 // Need christian
#define portafficheur6  42 // Need christian

#define bp 42  //Entree christian
#define servo 42  //Sortie chistian

#define portbtnmenu1 42 // Entree Christian
#define portbtnmenu2 42 // Entree Christian
#define portbtnretour 42 // Entree christian
int btnmenu1;
int btnmenu2;
int btnretour;

int fakepoid;
int croquettesnow;
#define croquettesdepart 300
int poidcroquettes;
int decrementpoid;

int afficheurman;

int heureauto;
int distribauto[24];

int heureauto1;
int heureauto2;

int heure = hour(); // FIXEME

int i;

Servo turnaround;
LiquidCrystal lcd(portafficheurRS, portafficheur2, portafficheur3, portafficheur4, portafficheur5, portafficheur6);

void setup() { //Init
  distribauto[9] = 1;
  distribauto[19] = 1;
  
	pinMode(bp, INPUT);
	pinMode(portbtnmenu1, INPUT);
	pinMode(portbtnmenu2, INPUT);
	pinMode(portbtnretour, INPUT);

	pinMode(servo, OUTPUT);

	lcd.begin(16, 2);

	turnaround.attach(servo);

	Serial.begin(9600);

	lcd.setCursor(0, 0);
	lcd.print("THE CAT FEEDER");

	Serial.println("Started");

	delay(2000);
	lcd.clear();
	menu();

    fakepoid = 300;
}

void loop() { //Boucle principale

	int btnmenu1 = digitalRead(portbtnmenu1); //Christian needed
	int btnmenu2 = digitalRead(portbtnmenu2); //Christian needed
	int btnretour = digitalRead(portbtnretour); //Christian needed
  
	btnconfig();

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