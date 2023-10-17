#include "Wata.h"
// assign pins globaly
using namespace std;
int wa_timer = 0;
int wa_time_min = 0;
int wa_timer_s = 0;
int wa_timer_hr = 0;
int sedmentation_dely = 0;
int sed_timer_s = 0;
int sed_time_min = 0;
int settled =0 ;
int val_timer = 0;
int val_time_min = 0;
int val_timer_s = 0;
int val_timer_hr = 0;

boolean flag;
Wata::Wata(byte pin10, byte pin11) {
  this->pin10 = pin10;
  this->pin11 = pin11;


  init();
}
void Wata::init() {
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  pinMode(this->pin10, OUTPUT);
  pinMode(this->pin11, OUTPUT);
  Serial.begin(9600);

}
ISR(TIMER2_COMPA_vect) {   //This is the interrupt request
  wa_timer++;
}



void Wata::run(Clara &clara) { // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
  /**
     creates one second delay to control the wata, the valve and 5 min delay for the chlorine to settle
  */
  if (wa_timer >= 200) {
    if (wa_state == HIGH) {
      
      wa_timer_s++;
      wa_timer = 0;
      
      
    // Serial.println(wa_timer_s);
    }
    
    if ( v_state == HIGH) {

      val_timer_s++;
      wa_timer = 0;
    }
    if (sedmentation_dely == HIGH) {
      sed_timer_s++;
      wa_timer = 0;
     // Serial.println("SED seconds");
    }

  }
  /**
     here we create one minute delay from the seconds that we created above
     for controlling the wata production,valve and a delay to settle the chlorine
  */

  if (wa_timer_s >= 55) {
    wa_time_min++;
    wa_timer_s = 0;
    
    //Serial.println("one minute delay for wata");
    clara.count_down_min-- ;
  }
  if (val_timer_s >= 55) {

    val_time_min++;
    val_timer_s = 0;
  }
  if (sed_timer_s >= 55) {
    sed_time_min++;
    sed_timer_s = 0;
   // Serial.println("one min");
  }


  if (wa_time_min >= 60) {
    wa_timer_hr++;
    wa_time_min = 0;
  }




  if ((wa_timer_hr >= 3) && (wa_state == HIGH)) { //creates a delay of two hours
    wa_timer_hr = 0;
    digitalWrite(this->pin10, LOW);
    wa_state = LOW;
    clara.wata_ready = 1;
    clara.ready_to_settle = 1;
    clara.count_down_min = 180;
    //Serial.println("wata production is complete");
    clara.set_state(4);


  }

  /**
     checks the if liquidlevel ==1 and wata_ready to start wata production
     @wata_ready -> prevents the system from producing a wata twice i.e after the valve is opened for 10 minutes wata_ready =1
     @clara.liquidlevel -> checks if there is liquid on the upper tank
  */
  if ((clara.liquidlevel_1 == 1) && (clara.wata_ready == 0)) {
    digitalWrite(this->pin10, HIGH); //start the wata production

    wa_state = HIGH;
   
  // Serial.println("wata is in production");

    clara.set_state(2);

    //display ont the lcd the process is abour to start
  }

  /*
     this if conditions creates a delay of 5 min  when ready_to_settle == 1 and sed_time_min >5
  */

  if ((sedmentation_dely == HIGH) &&  (sed_time_min >= 5)) {
    
    sedmentation_dely = LOW;
    clara.ready_to_settle = 0;
    settled = 1;
    

    sed_time_min = 0;
  }
  if (clara.ready_to_settle == 1) {
   
    
    sedmentation_dely = HIGH;
    clara.set_state(5);
     //Serial.println("wata is settling");
  }
  /**
     the if conditions below checks if clara.wata_ready ==1 (which will be on after 10 minutes of dely after checking if it settled)
      @v_state -> will be HIGH when clara.wata_ready ==1

  */


  if ((val_time_min >= 8) && (v_state == HIGH) ) {
    val_time_min = 0;

    digitalWrite(this->pin11, LOW);
    //Serial.println("Valve is closed");
    clara.set_state(3);
    v_state = LOW;
    clara.wata_ready = 0;
    settle_state = 0;
    settled =0;
    

  }


  if (settled == 1 && (clara.liquidlevel_2 == 0) ) {
    digitalWrite(this->pin11, HIGH);
     //Serial.println("valve is opened for 10 min");
    v_state = HIGH;
    valvecome = millis();
    clara.set_state(6);


  }


}
