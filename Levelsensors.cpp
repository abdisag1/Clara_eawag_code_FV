#include "Levelsensors.h"

// assign pins globaly
//let's say you want to read up to 100 values
const unsigned int numReadings = 20;
int level_1reading[numReadings];
unsigned int j = 0;
unsigned int x = 0;
unsigned int k = 0;

int level = 1;
int level_2reading[numReadings];
int level2 = 1;
int level_3reading[numReadings];
int level3 = 1;

int mytimer = 0;
int mytime_now = 0;
int mytimer_s = 0;

int levelcom1;
int levelcom2;
int levelcom3;

int Liquid_level_1;
Levelsensors::Levelsensors(byte pinA0, byte pinA1, byte pinA2) {
  this->  pinA0 = pinA0;
  this-> pinA1 = pinA1;
  this-> pinA2 = pinA2;
  init();
}

void Levelsensors::init() { // put your setup code here, to run once:

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  Serial.begin(9600);

}
ISR(TIMER1_COMPA_vect) {   //This is the interrupt request
  mytimer++;
}


void Levelsensors::run(Clara &clara)
{
  //Serial.println("helow from level");
  if (mytimer >= 500) {
    mytimer_s++;
    mytimer = 0;

    // Serial.print("taking sample");
    level_1reading[j] = digitalRead(A0);
    // Serial.println(digitalRead(A0));

    //  Serial.print(digitalRead(A0));
    level_2reading[x] = digitalRead(A1);

     //Serial.print(digitalRead(A1));

    level_3reading[k] = digitalRead(A2);
//    Serial.print(digitalRead(A2));

    /**
       compares 20 values from the liquid sensor 1 and saves it.
    */

    j++;
    if (j >= numReadings)
    {
      j = 0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array
    }
    //loop through the array comparing the other elements with the first
    for (int j = 1; j < 20; j++) {
      //Serial.println(level_1reading[j]);
      if ((level_1reading[0] != level_1reading[j]) || (level_1reading[0] == 0) || (level_1reading[j] == 0)) {

        level = 0;
        break;

      }
      else {
        level = 1;
      }
    }
    /**
       compares 20 values from the liquid level sensor 2 and saves the data in array
    */


    x++;

    if (x >= numReadings)
    {
      x = 0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array
    }
    //loop through the array comparing the other elements with the first
    for (int x = 1; x < 20; x++) {
      //Serial.println(level_2reading[x]);
      if ((level_2reading[0] != level_2reading[x]) || (level_2reading[0] == 0) || (level_2reading[x] == 0)) {

        level2 = 0;
        break;
      }
      else {

        level2 = 1;
      }
    }

 

  /**
     compares  the values from the third liquid level sensor and saves it .
  */
  k++;

  if (k >= numReadings)
  {
    k = 0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array
  }
  //loop through the array comparing the other elements with the first
  for (int k = 1; k < 20; k++) {
    // Serial.println(level_3reading[k]);
    if ((level_3reading[0] != level_3reading[k]) || (level_3reading[0] == 0) || (level_3reading[k] == 0)) {

      level3 = 0;
      break;
    }
    else {

      level3 = 1;
    }
  }
  }

  /**
     changes the state of the sensor every 20 seconds
  */



  if (mytimer_s >= 20) {

    mytimer_s = 0;
    mytime_now++;
    levelcom1 = level;

    clara.set_liquidlevel_1(levelcom1);
//    Serial.print("levelcom1");
//    Serial.println(levelcom1);
    levelcom2 = level2;

    clara.set_liquidlevel_2(levelcom2);

//    Serial.print("levelcom2");
//    Serial.println(levelcom2);
    levelcom3 = level3;
  
    clara.set_liquidlevel_3(levelcom3);
//    Serial.print("levelcom3");
//    Serial.println(clara.get_liquidlevel_3());





  }
}
