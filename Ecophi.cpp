

#include "Clara.h"

#include "Ecophi.h"
#define EcoRX       0  //Serial Receive pin
#define EcoTX        1  //Serial Transmit pin
#define EcoTxControl 2   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW
using namespace std;
int i = 0;
int timer = 0;
int time_now = 0;
int timer_s = 0;
int timer4 = 0;
float flowcommulative = 0;
float flowaverage = 0;
float orp_upper_commulative = 0;
float orp_orp_lower=0;
float orp_upper_fourmin_average =0;
float orp_lower_commulative = 0;
float orp_lower_fourmin_average =0;
  
Ecophi::Ecophi(byte pin0, byte pin1, byte pin2) {
  this->  pin0;
  this->  pin1;
  this->  pin2;

  init();
}
 

void Ecophi::init() {
// put your setup code here, to run once:
  TCCR0A = (1 << WGM01); //Set the CTC mode
  OCR0A = 0xF9; //Value for ORC0A for 1ms

  TIMSK0 |= (1 << OCIE0A); //Set the interrupt request
 // sei(); //Enable interrupt

  TCCR0B |= (1 << CS01); //Set the prescale 1/64 clock
  TCCR0B |= (1 << CS00);
  Serial.begin(9600);
  pinMode(EcoTxControl, OUTPUT);
  digitalWrite(EcoTxControl, RS485Transmit);  // Init Transceiver
  


}
ISR(TIMER0_COMPA_vect) {   //This is the interrupt request
  timer++;
}


void Ecophi::run(Clara &clara) {
  // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
  if (Serial.available() > 0) {
  }

  if (timer >= 500) {
    timer_s++;
    timer = 0;
    float flownow = clara.get_flow(); //get the flow each second from clara.get_flow()
    flowcommulative += flownow;
      
//   
//    Serial.println(timer_s);
   
  }

  if (timer_s >= 60) {
    time_now++;
    timer_s = 0;
    
    orp_upper_commulative =+ clara.upper_orp;
   
    
    orp_lower_commulative =+ clara.lower_orp;
    //Serial.println("this is 1 min");
  }


  if (time_now >= 1) {
    time_now = 0;

    flowaverage = flowcommulative / 252; //get the average of the four minutes form the commulative of the each second 164
    orp_upper_fourmin_average = orp_upper_commulative/4;
    orp_lower_fourmin_average = orp_lower_commulative/4;
    
    float eflow = flowaverage;
    int  elevel_1 = clara.get_liquidlevel_1();
    int   elevel_2 = clara.get_liquidlevel_2();
    int   elevel_3 = clara.get_liquidlevel_3();
    float evotage = clara.get_voltage();
    float upper_orp_reading = orp_upper_fourmin_average;
    float lower_orp_reading = orp_lower_fourmin_average;
    float average_pump_time = clara.global_pump_time_total/clara.four_minutes_pump_count;
    float dummy_chlorine = clara.calculated_Cl;
    
    //float cl_pumped = clara.pump_count * clara.volume_per_pump;
    //Serial.println("this is 4 min");
    //Serial.println(flow);
    
    Serial.print(";");
    //Serial.print("clara:");
    Serial.print(eflow);
    Serial.print(",");
    Serial.print(evotage);
    Serial.print(",");
    Serial.print(elevel_1);
    Serial.print(",");
    Serial.print(elevel_2);
    Serial.print(",");
    Serial.print(elevel_3);
    Serial.print(",");
    Serial.print(orp_upper_fourmin_average);
    Serial.print(",");
    Serial.print(orp_lower_fourmin_average);
    Serial.print(",");
    Serial.print(average_pump_time);
    Serial.print(",");
    Serial.print(clara.pump_count);
    Serial.print(",");
    Serial.print(dummy_chlorine);
    Serial.print(":");
    Serial.println("");
    
      
    flowcommulative = 0;
    flowaverage = 0;
    orp_upper_fourmin_average =0;
    orp_lower_fourmin_average=0;
    orp_upper_commulative = 0;
    orp_lower_commulative = 0;
    clara.commulative_pump_time =0;
    clara.four_minutes_pump_count = 0;
  
   
    
  }

}
