/*
 * 
 * FAN CONTROL AGAINST MOLD
 * 
 */

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;

//
// when cold season often dry the wall even
// if humidity doesn't grow over the BAD_EVEL
bool COLD_SEASON = false;

int fan_pin = 5;
int sensor = A0;
int BAD_LEVEL = 27; // % HUMIDITY

//
// NOVEMBER
// MEASURED HUM 25% IN THE ROOM - 19/11/2022 11:16 
// BEHIND WARDROBE 37%
//

int SPEED = 0;
int readingDelta = 30000; // 30 sec

unsigned long T = 60UL*1000UL*60UL * (COLD_SEASON ? 1UL : 1UL); // 1 HOUR main timer
unsigned long T2 = 60UL*1000UL*60UL * (COLD_SEASON ? 1UL : 3UL); // 1 or 3 HOURS off when no BAD_LEVEL

byte fan_status = 0;
void setup() {
  pinMode(fan_pin, OUTPUT);
  analogWrite(fan_pin,255);
  delay(5000);
  pinMode(sensor, INPUT);

  Serial.begin(9600);
  Serial.println(".");
  Serial.println("START FAN CONTROL --------------------");

}

unsigned long timer = 0;

void loop() {
  
    DHT.read(sensor);
  
    if(fan_status==0 && DHT.humidity < BAD_LEVEL  && millis() > timer ) {
        timer = millis() + T;
        fan_status=1;
        SPEED = 153;
        analogWrite(fan_pin, SPEED);
    }

    if(fan_status==0 && DHT.humidity >= BAD_LEVEL) {
        fan_status= 2;
        SPEED = 255;
        analogWrite(fan_pin, SPEED);
    }

    if(fan_status==2 && DHT.humidity < BAD_LEVEL) {
        fan_status= 0;
        timer = millis() + T2;
        SPEED = 0;
        analogWrite(fan_pin, SPEED);
    }

    if(fan_status==1 && DHT.humidity >= BAD_LEVEL) {
        fan_status= 2;
        SPEED = 255;
        analogWrite(fan_pin, SPEED);
        timer = millis() + T;
    }

    if(fan_status==1 && DHT.humidity < BAD_LEVEL && millis() > timer) {
        fan_status= 0;
        timer = millis() + T2;
        SPEED = 0;
        analogWrite(fan_pin, SPEED); 
    }

    if(fan_status==2 && DHT.humidity >= BAD_LEVEL) {
      timer = millis() + T;
    }


    Serial.print("HUM ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("T ");
    Serial.print(DHT.temperature); 
    Serial.print("°C  ");
    Serial.print("FAN ");
    Serial.print(fan_status);
    Serial.print("  SPEED ");
    Serial.print(SPEED);
    unsigned long t = (timer - millis()) / 1000UL;
    Serial.println("  TIMER " + (String)t + "sec");

    delay(readingDelta);
  
  
}
