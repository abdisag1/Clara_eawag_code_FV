/** Author Abdisa Gemechu 
 *  contact myabdisa@gmail.com 
 * This is the main function of the system .
  */

#include "Clara.h"
#include "Flowmeter.h"
#include "Pump.h"
#include "Levelsensors.h"
#include "Wata.h"
#include "Voltagesensor.h"
#include "Ecophi.h"
#include "Display.h"
#include "Orpsensors.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Display driver
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display





using namespace std;
static byte pinA0 = A0;
static byte pinA1 = A1;
static byte pinA2=  A2;
static byte pinA3=  A3;
static byte pinA4=  A4;
static byte pinA5=  A5;

static byte pin0 = 0;
static byte pin1 = 1;
static byte pin2 = 2;
static byte pin3 = 3;
static byte pin4=  4;
static byte pin5= 5;

static byte pin7 = 7;
static byte pin8 = 8;
static byte pin9 = 9;
static byte pin10 = 10;
static byte pin11 = 11;
static byte pin12 = 12;



static byte pin13=13;

static byte pin18=18;
static byte pin19=19;
static byte pin21=21;
static byte pin22=22;

// This really has to be at this place, if placed in the the setup-function of the code, it will not work!
Clara clara;

Flowmeter flowmeter_used(pin3);
Pump pump_used(pin12);
Wata wata_used(pin10,pin11);

Levelsensors level_sensors_used(pinA0,pinA1,pinA2);

Voltagesensor voltage_sensorused(pinA3);
Orpsensors orp_sensorused(pinA4,pinA5);

Ecophi ecophi_sensorused(pin0,pin1,pin2);
Display display_used(pin13);

void setup() { 
  
  lcd.init();
  lcd.begin(20,4,LCD_5x8DOTS); 
  lcd.backlight();
  Serial.begin(9600);

  }


void loop() {
  
 
  
  flowmeter_used.run(clara);
  pump_used.run(clara);
  voltage_sensorused.run(clara);
  ecophi_sensorused.run(clara);
  wata_used.run(clara);
 
  level_sensors_used.run(clara);
 
 
  voltage_sensorused.run(clara);
 orp_sensorused.run(clara);
  
 
  display_used.run(clara,lcd); 
  
  
  }
