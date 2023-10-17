#include "Voltagesensor.h"
// assign pins globaly

Voltagesensor::Voltagesensor(byte pin) {
  this->  pin; 
 
  init();
} 
void Voltagesensor::init() {
  pinMode(pin,INPUT);
 // Serial.begin(9600);
}
//l_delay
void Voltagesensor::run(Clara &clara) { // Has to be passed as a reference! See p. 255 in "Grundkurs C++"

value = analogRead(voltageSensor);
  vOUT = (value * 6) / 1024.0;
  voltage = vOUT / (R2/(R1+R2));
  //Serial.print("Voltage = ");
  //Serial.println(voltage);
  //delay(500);
  clara.set_voltage(voltage);
 batterypercentage = ((voltage- low) / (high - low)) * 100;

// delay(4000);
 

 if (batterypercentage >= 100){
  
  batterypercentage=100;
  //Serial.print("battery percentage= ");
 //Serial.println(batterypercentage);
 }
 else if (batterypercentage <=0){ 
  batterypercentage=0;
   //Serial.print("battery percentage= ");
 //Serial.println(batterypercentage);
 }
 else { 

   //Serial.print("battery percentage= ");
 //Serial.println(batterypercentage);
 }
      

}
