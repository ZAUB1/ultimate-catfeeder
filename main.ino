#include <Time.h> // Synchro tps avec l'ordi
#include <LiquidCrystal.h> // Lib afficheur liquid
#include <Servo.h> // Lib servo
#include <Wire.h>

//Config
int maxdistribparjour = 0;
#define valservo // A chercher

//Port afficheur liquid
#define portafficheurRS // Need christian
#define portafficheur2 // Need christian
#define portafficheur3 // Need christian
#define portafficheur4 // Need christian
#define portafficheur5 // Need christian
#define portafficheur6 // Need christian

#define bp //Entree christian
#define btnreglage // Entree chistian
#define servo //Sortie chistian

#define btnmenu1 //Christian needed
#define btnmenu2 //Christian needed
#define btnretour //Christian needed

int fakepoid;
int croquettesnow;
#define croquettesdepart 300

int afficheurman;

int heureauto;
int heureauto1;
int heureauto2;
int heure = hour();

Servo turnaround;
LiquidCrystal lcd(portafficheurRS, portafficheur2, portafficheur3, portafficheur4, portafficheur5, portafficheur6);

void setup() { //Init
	pinMode(bp, INPUT);

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

	heureauto();
	btnconfig();

	int bpactiv = digitalRead(bp);
    int decrementpoid = random(45, 65);

	if (heureauto == 1)
	{
		manger();

		Serial.println("LA BOUFFE");

		heureauto = 0;
        fakepoid = fakepoid - decrementpoid;

        Serial.println("Var poid : ");
        Serial.println(fakepoid);
	}
	else if (bpactiv == 1 && distribparjour <= maxdistribparjour && heureauto = 0)
	{
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
    if (btnmenu1 == 1)
    {
        lcd.clear();
        reglages();
    }
    else if (btnmenu2 == 1)
    {
        lcd.clear();
        poidcroquettes();
    }
    else if (btnretour == 1)
    {
        lcd.clear();
        menu();
    }
}

void btnconfig() {
	btnplus = digitalRead(btnreglage);
	
	if (btnplus == HIGH)
	{
		addconfig();
	}
}

void addconfig() {
	for (maxdistribparjour = 1, 5, maxdistribparjour = maxdistribparjour++)
	{
		Serial.println(maxdistribparjour);

		delay(50);

		lcd.noDisplay();
		lcd.display();
	}
}

void heureauto() {
	if (heure == 7 or heure == 19)
	{
		heureauto = 1;
	}
	else
	{
		heureauto = 0;
	}
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
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Heures");
    lcd.setCursor(1, 3);
    lcd.print("1")
    lcd.setCursor(0, 8);
    lcd.print("Manuel");
    lcd.setCursor(1, 11);
    lcd.print("2");

    if (btnmenu1 == 1)
    {
        lcd.clear();
        rglheures();
    }
    else if (btnmenu2 == 1)
    {
        lcd.clear();
        display();
    }
    else if btnretour == 1
    {
        lcd.clear();
        menu();
    }
}
