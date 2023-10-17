#include "Pump.h"
#define ON 1
#define OFF 0
#define PUMP_INTERVAL 10000 // Volume of water in milliliter to pass in the flowmeter until a chlorination occurs, [ml]
#define PUMP_RATE 400 // Flow rate of the pump, [ml/min]
#define CHLORINE_CONCENTRATION 1.8  // Desired initial concentration of chlorine in the treated water, [mg/l]
#define WATA_CONCENTRATION 4.5 // Concentration of the WATA-Solution, [g/l]

using namespace std;
Pump::Pump(byte pin) {
  this->pin = pin;
  this->state = OFF;
  this->pumpInterval = PUMP_INTERVAL; // Volume of water in milliliter to pass in the flowmeter until a chlorination occurs, [ml]
  this->pumpRate = PUMP_RATE; // Flow rate of the pump, [ml/min]
  this->chlorineConcentration = CHLORINE_CONCENTRATION; // Desired initial concentration of chlorine in the treated water, [mg/l]
  this-> wataConcentration = WATA_CONCENTRATION; // Concentration of the WATA-Solution, [g/l]
  this-> currentTime;
  this->cloopTime;
  this->delta_t;
  this->delta_V;
  this->pump_time_started;
  this->pump_time_total;


  init();
}
void Pump::init() {
  pinMode(this->pin, OUTPUT);
  currentTime = millis();
  cloopTime = currentTime;
}

void Pump::check_pump(){
  if(get_state()==ON){
    if(millis()>pump_time_started+pump_time_total){
      set_state(OFF);
    }
    // If millis() starts from 0 again
    else if(millis()<pump_time_started){
      pump_time_started = millis();
    }
  }
}
void Pump::pump_chlorine(Clara &clara){
 
  
    clara.global_pump_time_total = chlorineConcentration * pumpInterval*60 / (pumpRate*wataConcentration);
    clara.global_pump_time_total= clara.global_pump_time_total*1.5;
    pump_time_total = clara.global_pump_time_total;
//    Serial.print("clara.global_pump_time_tota");
//    Serial.println(clara.global_pump_time_tota);
    pump_time_started = millis();
    delta_V = 0;
    unsigned long numerator = clara.global_pump_time_total*pumpRate;
    unsigned long seconds_per_hour = 60;
    unsigned long milliseconds_per_seconds = 1000;
    unsigned long denominator = seconds_per_hour*milliseconds_per_seconds;
    unsigned long chlorine_volume_pumped = numerator/denominator;
    // pump_time_started= 0;
  int time_now=0;
  //set_state(ON);
  
 /*  if(pump_time_started >=pump_time_started + clara.global_pump_time_tota ){
        pump_time_started +=  clara.global_pump_time_tota;
         set_state(ON); 
    } */
  /*set_state(ON);*/
  
  if(int(chlorine_volume_pumped)>= 0){
   //clara.decrease_chlorine_volume(int(chlorine_volume_pumped));
   
      set_state(ON);
      /// we can increase the counter for the pump here
      // clara.pump_count++;
       }
    

}

void Pump::run(Clara &clara) { // Has to be passed as a reference! See p. 255 in "Grundkurs C++"

   currentTime = millis();
  // pump_chlorine(clara);
   // Every second, re-calculate the flow already passed by since the last chlorination
   if(currentTime >= (cloopTime + 1000))
   {  
      delta_t = (currentTime - cloopTime)/1000; // delta t in s
      cloopTime = currentTime; // Updates cloopTime
      double ml_s = clara.get_flow()*1000/60;
      delta_V = ml_s*delta_t + delta_V;
      
      if(delta_V > pumpInterval){
        pump_chlorine(clara);
      }
   }

  check_pump();
  
  if(state == ON && ((clara.get_liquidlevel_3()==1))){
    digitalWrite(this->pin, HIGH);
    clara.pump_count++;
////    
//   Serial.println("Pumped");
   

    if(millis()>pump_time_started+clara.global_pump_time_total){
      set_state(OFF);}

    
  }

  else{
    digitalWrite(this->pin, LOW);
  
  }
}

void Pump::set_state(bool state){
  this->state = state;
}

bool Pump::get_state(){
  return this->state;
}
