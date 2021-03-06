#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C address 0x3F, 16 column and 2 rows

int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin

float duration_us, distance_cm;  // command for ultrasonic sensor


SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;//It includes a message system for exchanging information between
                //GPS navigation receivers and consumers of navigation information.


SoftwareSerial SIM800(6, 7);
void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started

  lcd.init();               // initialize the lcd
  lcd.backlight();          // open the backlight 
  pinMode(trigPin, OUTPUT); // config trigger pin to output mode
  pinMode(echoPin, INPUT);  // config echo pin to input mode


  SIM800.begin(9600);
  SIM800.println("AT");
  

}

void loop()
{
  while(serial_connection.available())//While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:"); //Подсчет Спутников
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:"); //Широта
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:"); //Долгота
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:"); //Скорость миль в час
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());//Высота футов
    Serial.println("");

    
    // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  lcd.clear();
  lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("Distance: ");
  lcd.print(distance_cm);

  delay(500);
  }

  if (SIM800.available())
    Serial.write(SIM800.read());
  if (Serial.available())
    SIM800.write(Serial.read());
}
