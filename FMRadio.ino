/*
  FMRadio.ino Doctype="Arduino 1.8.5"

  Aquest programa permet escoltar una emisora de radio gràcies a la placa TEA5767.
  També mostra per pantalla LCD algunes dades importants, com la freqüència o la qualitat.
  Totes les configuracions es fan per mitjà de push buttons (que només tenen una posició).
*/

#include <TEA5767N.h>  //Per a descarregar la llibreria https://github.com/mroger/TEA5767
#include <LiquidCrystal.h> //Aquesta llibreria ja ve preinstal·lada a la placa Arduino.

//Es declaren els pins on es troba connectada la pantalla LCD.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Es declaren els botons i variables per a canviar la freqüència.
const int pinup = 9, pindown = 8;
int frequp = 0, freqdown = 0;

//S'inicialitza el mòdul receptor.
TEA5767N radio = TEA5767N();

//Es declaren les variables que s'usaran en el programa principal (void loop()).
float frequency = 0;
float oldFrequency = 0;
int signalStrength = 0;


void setup() {
  //Aquí es posa el codi que només cal que s'executi al engegar la placa.

  //S'inicialitza la pantalla i la antena.
  radio.setMonoReception();
  radio.setStereoNoiseCancellingOn();
  lcd.begin(16, 2);
  lcd.print("Freq:   ---.- Hz");
  lcd.setCursor(0, 1);
  lcd.print("Sig: -/5  ------");

  //S'inicialitzen els botons de canvi de freqüència.
  pinMode(pinup, INPUT);
  pinMode(pindown, INPUT);

  //Es recuperara la freqüència preferida, i es posa a l'antena.
  frequency = 92.2;
  radio.selectFrequency(frequency);
  printFrequency(frequency);
  printSignalStrength();
  printStereo();
}


void loop() {

  //Tria de freqüència.
  oldFrequency = frequency;
  frequp = digitalRead(pinup);
  freqdown = digitalRead(pindown);
  if (frequp == HIGH) {
    frequency = oldFrequency + 0.1;
    radio.selectFrequency(frequency);
    printFrequency(frequency);
  }
  else if (freqdown == HIGH) {
    frequency = oldFrequency - 0.1;
    radio.selectFrequency(frequency);
    printFrequency(frequency);
  }

  //Actualitza la qualitat del so a la pantalla.
  printSignalStrength();
  printStereo();

  //Espera una estona per a tornar a començar el bucle.
  delay(10);
}

void printFrequency(float frequency) {
  //Aquesta funció mostra/actualitza per la pantalla led (liquid crystal),
  //la freqüència en que la antena està captant el senyal de radio.
  String frequencyString = String(frequency, 1);
  lcd.setCursor(8, 0);
  if (frequency < 100) {
    lcd.print(" ");
    lcd.print(frequencyString);
  }
  else {
    lcd.print(frequencyString);
  }
}


//Es defineixen les funcions secundàries del programa.

void printStereo() {
  //Aquesta funció mostra/actualitza per la pantalla led (liquid crystal),
  //el tipus de senyal radio que rep la antena (mono/stereo).
  boolean isStereo = radio.isStereo();
  lcd.setCursor(10, 1);
  if (isStereo) {
    lcd.print("STEREO");
  }
  else {
    lcd.print("  MONO");
  }
}

void printSignalStrength() {
  //Aquesta funció mostra/actualitza per la pantalla led (liquid crystal),
  //en una escala del 0 al 5, la qualitat del senyal de radio que rep la antena.
  signalStrength = radio.getSignalLevel();
  lcd.setCursor(5, 1);
  if (signalStrength >= 17) {
    lcd.print("5");
  }
  else if (signalStrength >= 14 && signalStrength < 17) {
    lcd.print("4");
  }
  else if (signalStrength >= 11 && signalStrength < 14) {
    lcd.print("3");
  }
  else if (signalStrength >= 8 && signalStrength < 11) {
    lcd.print("2");
  }
  else if (signalStrength >= 5 && signalStrength < 8) {
    lcd.print("1");
  }
  else {
    lcd.print("0");
  }
}
