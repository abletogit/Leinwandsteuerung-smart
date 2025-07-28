#include <RCSwitch.h>
#include <SoftwareSerial.h>

RCSwitch mySwitch = RCSwitch();

// RX auf Pin 2, TX auf Pin 3
SoftwareSerial mySerial(2, 3);

const unsigned long CODE_RUNTER = 6663426;
const unsigned long CODE_STOP   = 6663428;
const unsigned long CODE_HOCH   = 6663432;
const byte BITLAENGE = 24;
const byte PROTOKOLL = 1;

void setup() {
  Serial.begin(9600);          // USB-Seriell (für Debugging am PC)
  mySerial.begin(9600);        // SoftwareSerial auf Pins 2 (RX) und 3 (TX)
  
  mySwitch.enableTransmit(5);
  mySwitch.setProtocol(PROTOKOLL);
  mySwitch.setPulseLength(320);
  mySwitch.setRepeatTransmit(10);

  Serial.println("Arduino bereit, warte auf Befehle vom ESP32...");
}

void loop() {
  if (mySerial.available()) {            // Prüfe SoftwareSerial (Pins 2 und 3)
    String befehl = mySerial.readStringUntil('\n');
    befehl.trim();

    Serial.print("Empfangen: ");
    Serial.println(befehl);              // Ausgabe für Debugging

    if (befehl == "-1") {
      Serial.println("runterfahren");
      mySwitch.send(CODE_RUNTER, BITLAENGE);
    } 
    else if (befehl == "1") {
      Serial.println("hochfahren");
      mySwitch.send(CODE_HOCH, BITLAENGE);
    } 
    else if (befehl == "0") {
      Serial.println("ich mach jetzt gar nix mehr");
      mySwitch.send(CODE_STOP, BITLAENGE);
    }
     else if (befehl == "-2") {
      Serial.println("kinomode startet");
      mySwitch.send(CODE_HOCH, BITLAENGE);
      delay(50000);
      mySwitch.send(CODE_RUNTER, BITLAENGE);
      delay(28000);
      mySwitch.send(CODE_STOP, BITLAENGE);
      Serial.println("Kinosaal ist vorbereitet");


    } 
  }
}
