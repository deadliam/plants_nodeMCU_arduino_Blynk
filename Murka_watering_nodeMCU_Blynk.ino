
//  Blynk.begin(auth, ssid, pass);
//  // You can also specify server:
//  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
//  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

char auth[] = "GIHSR-a0LAaLKnjLf0ecjVMHLCarT_nP";
const char *ssid =  "Xiaomi_236D";
const char *pass =  "ellisfromlos";

SimpleTimer timer;
WidgetLED led1(V6);

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors

int firstVal, secondVal, thirdVal, fourthVal; // sensors 
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
  
}

void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L,sensorvalue1); 
  timer.setInterval(1000L,sensorvalue2); 
  timer.setInterval(1000L,sensorvalue3); 
  timer.setInterval(1000L,sensorvalue4);
  
  timer.setInterval(1000L,trigger);
}

void loop() {
  if (Serial.available() == 0 ) {
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
  }
   
  if (Serial.available() > 0 ) {
    rdata = Serial.read(); 
    myString = myString + rdata; 
    // Serial.print(rdata);
    if( rdata == '\n') {
      //  Serial.println(myString); 
      // Serial.println("fahad");
      // new code
      String l = getValue(myString, ',', 0);
      String m = getValue(myString, ',', 1);
      String n = getValue(myString, ',', 2); 
      String k = getValue(myString, ',', 3);
    
    
      firstVal = l.toInt();
      secondVal = m.toInt();
      thirdVal = n.toInt();
      fourthVal = k.toInt();
    
      myString = "";
      // end new code
    }
  }
}

void sensorvalue1() {
  int sdata = firstVal - 240;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
}

void sensorvalue2() {
  int sdata = secondVal - 240;
  Blynk.virtualWrite(V3, sdata);
}

void sensorvalue3() {
  int sdata = thirdVal - 240;
  Blynk.virtualWrite(V4, sdata);
}

void sensorvalue4() {
  int sdata = fourthVal - 240;
  Blynk.virtualWrite(V5, sdata);
}

void trigger() {
  if (firstVal > 600 || secondVal > 600 || thirdVal > 600 || fourthVal > 600) {
    led1.on();
  } else {
    led1.off();
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
