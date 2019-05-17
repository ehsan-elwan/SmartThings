/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - NodeMCU board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

//  Mouvement Sensor: 000111010101010111111000 353 dec:1922552  z-10   chambre
//                    100110100001110111111000     dec:10100216 z-2-4  salon tv
//                    010001000101010001010000     dec:4478032  z-2-2   entree
//                    111001011101100111111000     dec:15063544 z-2-3  salle a manger
//                    000100000001100111111000     dec:1055224 z-3-1   garage portail
//                    000011110110001011111000     dec:1008376  z-3-2 garage porte
//
/*

   Decimal: 13161688 (24Bit) Binary: 110010001101010011011000  JAWAD
   Decimal: 16057792 (24Bit) Binary: 111101010000010111000000  EHSAN
   Decimal: 13270504 (24Bit) Binary: 110010100111110111101000  007

     portail: 100000110000000000000000 288 dec:8585216   || 111110100010000011101000 284 dec:16392424
  Clio A:111110100010000011100001 285 dec:16392417
  Clio B:111110100010000011100010 286 dec:16392418
  Fiat ouv:110110100010010011101001 285 dec:14296297
  Fiat ferm:111010101010001001101001 286 dec:15377001
  Fiat ouvFen:100100101011010001100101 285 dec:9614437
  Fiat fermFen:111011111000101100111001 286 dec:15698745
  B button: 100000111111110011010100 dec:8649940
  Zenger sonnet Decimal: 14671871 (24Bit) Binary: 110111111101111111111111 Tri-State: 1F111F111111 PulseLength: 218 microseconds Protocol: 1

*/
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include "config.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <RCSwitch.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

#define jawad 13161688
#define ehsan 16057792
#define jean_clio 13270504
#define buttonB 8649940
#define clioA 16392417
#define clioB 16392418
#define portail1 8585216
#define portail2 16392424
#define sonnet 14671871

#define openBox D7
#define rxPin D6
#define newMail D8
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = auth_ext;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = wifiSSID;
char pass[] = wifiPass;

bool inbox = false;
bool checkBox = true;

String data;
String timey;

uint8_t pin2set;

RCSwitch mySwitch = RCSwitch();
BlynkTimer timer;
WidgetRTC rtc;


void new_mail() {
  int tmp = digitalRead(newMail);
  if (tmp) {
    Serial.println("Hey got new mail");
    Blynk.virtualWrite(V0, 1);
    inbox = true;
    checkBox = true;
  }

}

void openbox() {
  int tmp = digitalRead(openBox);
  if (checkBox && tmp) {
    checkBox = false;
    Blynk.virtualWrite(V0, 0);
    timer.setTimeout(20000L, callback);
  }

}

void callback() {
  inbox = false;
}

#define jawad 13161688
#define ehsan 16057792
#define jean_clio 13270504

void readRx433() {
  if ( mySwitch.available()) {
    int rxValue =  mySwitch.getReceivedValue();
    switch (rxValue) {
      case buttonB:
        Blynk.notify("Alarme: B");
        break;
      case sonnet:
        Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=4&stream=0.sdp");
        Blynk.virtualWrite(V10, 1);
        Blynk.notify("Sonnet!");
      case jawad:
        Blynk.virtualWrite(V1, HIGH);
        pin2set = V1;
        timer.setTimeout(1000L, setBack);
        break;
      case ehsan:
        Blynk.virtualWrite(V2, HIGH);
        pin2set = V2;
        timer.setTimeout(1000L, setBack);
        break;
      case jean_clio:
        Blynk.virtualWrite(V3, HIGH);
        pin2set = V3;
        timer.setTimeout(1000L, setBack);
        break;

      default:
        break;

    }

    mySwitch.resetAvailable();
  }

}

void setBack() {
  Blynk.virtualWrite(pin2set, 0);
}

void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  timey = String(hour()) + ":" + minute();
  data = String(day()) + "/" + month() + "/" + year();

}

BLYNK_CONNECTED() {
  rtc.begin();
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(newMail, INPUT);
  pinMode(openBox, INPUT_PULLUP);
  mySwitch.enableReceive(rxPin);


  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  delay(2000);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 0, 253), 64214);

}

void loop()
{
  Blynk.run();
  timer.run();
  readRx433();
  if (!inbox) {
    new_mail();
  }
  if (inbox) {
    openbox();
  }

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
