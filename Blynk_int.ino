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

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include "config.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <RCSwitch.h>
#include "DHT.h"


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = auth_int;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = wifiSSID;
char pass[] = wifiPass;

bool alarm = false;

float temp = 0;
float hum = 0;

uint8_t activeCam;

RCSwitch mySwitch = RCSwitch();
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WidgetTerminal terminal(V3);

void readEnv() {
  Blynk.virtualWrite(V0, dht.readTemperature());
  Blynk.virtualWrite(V1, dht.readHumidity());
  Blynk.virtualWrite(V2, dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false));
}


void readRx433() {
  if (alarm && mySwitch.available()) {
    int rxValue =  mySwitch.getReceivedValue();
    switch (rxValue) {
      case Sen_Z_1_0:
        Blynk.notify("Alarme: Chambres");
        break;
      case Sen_Z_2_4:
        Blynk.notify("Alarme: Salon TV");
        break;
      case Sen_Z_2_2:
        Blynk.notify("Alarme: Entrée");
        break;
      case Sen_Z_2_3:
        Blynk.notify("Alarme: Salle à manger");
        break;
      case Sen_Z_3_1:
        Blynk.notify("Alarme: Garage portail");
        break;
      case Sen_Z_3_2:
        Blynk.notify("Alarme: Garage porte");
        break;
      default:
        terminal.print("Alarme inconnue: ");
        terminal.println(rxValue);
        break;

    }



    mySwitch.resetAvailable();
  }

}


BLYNK_WRITE(V4)
{
  int pinData = param.asInt();

  if (pinData) {
    alarm = true;

  } else {
    alarm = false;
  }

}

BLYNK_WRITE(V7)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=1&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V7;
  }
}

BLYNK_WRITE(V8)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=2&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V8;
  }
}

BLYNK_WRITE(V9)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=3&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V9;
  }
}

BLYNK_WRITE(V10)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=4&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V10;
  }
}

BLYNK_WRITE(V11)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=5&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V11;
  }
}

BLYNK_WRITE(V12)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=6&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V12;
  }
}

BLYNK_WRITE(V13)
{
  if (param.asInt()) {
    Blynk.setProperty(V6, "url", "rtsp://192.168.0.13/user=admin&password=007ipcam&channel=7&stream=0.sdp");
    Blynk.virtualWrite(activeCam, 0);
    activeCam = V13;
  }
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  mySwitch.enableReceive(rxPin);

  timer.setInterval(env_m_time, readEnv);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 0, 253), 64214);
}

void loop()
{
  Blynk.run();
  timer.run();
  readRx433();
  readEnv();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
