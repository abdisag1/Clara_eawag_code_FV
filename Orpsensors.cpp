#include "Orpsensors.h"
#include "orp_grav.h"


Gravity_ORP upper_sensor = Gravity_ORP(A4);
Gravity_ORP lower_sensor = Gravity_ORP(A5);


/////////////////////////////////////////////////////////
/////////////// VARIABLES ///////////////////////////////
/////////////////////////////////////////////////////////

/////////////// measurement value update ////////////////

// current ORP readings
int upper_orp_curr;  // GLOBAL!!!!!!!!!!
int lower_orp_curr;  // GLOBAL!!!!!!!!!!
// array of the last ORP readings, current_array_length is the number of readings which are averaged
#define current_array_length       10
int upper_orp_array [current_array_length];
int lower_orp_array [current_array_length];

// average of array
int     upper_avg;
int     lower_avg;

// calculation of chlorine concentration
float slope = 0.007486179406175297;
float intercept = -5.450706689774224;
float calculated_Cl_local;  // GLOBAL!!!!!!!!!!


/////////////// feedback loop ////////////////

// (1)
// array for calculating the difference over time
boolean condition_1 = 0;
#define lower_orp_long_array_length  30
int lower_orp_long_array [lower_orp_long_array_length];
int lower_orp_change_over_time;
int max_lower_orp_change_over_time = 5;

// local pump_count variables
int pumpcount_old;
int pumpcount_new;
int pumpcount_diff;

// (2)
// dispensed 3 times in the last 3 min?
boolean condition_2 = 0;
#define pumpcount_array_length  180
int pumpcount_array[pumpcount_array_length];
int pumpcount_sum;

// (3)
// dispensed at least 4 times since last change of pumptime?
boolean condition_3 = 0;
int min_dispensions_to_wait = 4;
int pumpcount_since_last_dispensing = 0;

// define maximum and minimum desired chlorine concentration
float maximum_calculated_Cl = 5;
float minimum_calculated_Cl = 0.5;

// pumptime parameters
int new_pumptime;
int min_pumptime = 1000;
int max_pumptime = 5000;   //would be nice to make this dependant on flow-interval

/////////////////////////////////////////////////////////
/////////////// FUNCTIONS ///////////////////////////////
/////////////////////////////////////////////////////////

Orpsensors::Orpsensors(byte pinA4, byte pinA5) {
  this->  pinA4 = pinA4;
  this-> pinA5 = pinA5;

  init();
}

void Orpsensors::init() {

  Serial.begin(9600);
}



void Orpsensors:: update_measurement_values () {
  // new measurement value
  upper_orp_curr = upper_sensor.read_orp();
  lower_orp_curr = lower_sensor.read_orp();

  // update upper and lower arrays, oldest at position [0]
  for (int i = 1; i < current_array_length; i++) {
    upper_orp_array[i - 1] = upper_orp_array[i];
    lower_orp_array[i - 1] = lower_orp_array[i];
  }
  upper_orp_array[current_array_length - 1] = upper_orp_curr;
  lower_orp_array[current_array_length - 1] = lower_orp_curr;

  // calculate average and range
  int upper_sum = 0;
  int lower_sum = 0;
  for (int x = 0; x < current_array_length; x++) {
    upper_sum += upper_orp_array[x];
    lower_sum += lower_orp_array[x];
  }

  upper_avg = upper_sum / current_array_length;
  lower_avg = lower_sum / current_array_length;

  // calculate the Chlorine from averaged ORP readings
  calculated_Cl_local= pow(10, lower_avg*slope + intercept) - pow(10, upper_avg*slope + intercept);


  // debug
//  Serial.print("upper ORP: ");
//  Serial.print(upper_avg);
//  Serial.print("    lower ORP: ");
//  Serial.print(lower_avg);
//  Serial.print("    calculated chlorine: ");
//  Serial.print(calculated_Cl_local);
//  Serial.println("");
}



void Orpsensors:: update_conditions_to_change_pumptime () {
  // three conditions to check if the system is ready to change pumptime:
  // (1) is the lower ORP signal stable?
  // (2) has the pump been dispensing?
  // (3) has the pump dispensed since last change of pumptime?

//// (1) check if lower ORP signal is stable:
  // update range array with new value, oldest at position [0]
  for(int i=1; i<lower_orp_long_array_length; i++){
      lower_orp_long_array[i-1] = lower_orp_long_array[i];
  }
  lower_orp_long_array[lower_orp_long_array_length-1] = lower_avg;

  // update ORP difference
  lower_orp_change_over_time = lower_avg - lower_orp_long_array[0];  

  // update condition 1
  if (lower_orp_change_over_time < max_lower_orp_change_over_time){
    condition_1 = 0;
  }else{
    condition_1 = 1;
  }


  // updating the local pumptime count for conditions (2) and (3)
  //pumpcount_new = clara.pumpcount;
  pumpcount_diff = pumpcount_new - pumpcount_old;
  pumpcount_old = pumpcount_new;
  // catch overflow of clara.pumpcount neccesary?

//// (2) check if the pump has been dispensing:
  //updating the array that keeps track of the pumpcount in the last (pumpcount_array_length/60) min
  for(int i = 1; i<pumpcount_array_length; i++){
      pumpcount_array[i-1] = pumpcount_array[i];
  }
  pumpcount_array[pumpcount_array_length-1] = pumpcount_diff;
  for(int i=0; i<pumpcount_array_length; i++){
    pumpcount_sum += pumpcount_array[i];
  }
//  Serial.print("    pump Count sum: ");
//  Serial.println(pumpcount_sum);
  if (pumpcount_sum >= 3){
    condition_2 = 1;
    }
  else{
    condition_2 = 0;
    }

//// (3) check if the pump has dispensed since the last change of pump_time:
  // update of the pumptimecount since last dispensing
  pumpcount_since_last_dispensing += pumpcount_diff;
  if (pumpcount_since_last_dispensing > min_dispensions_to_wait){
    condition_3 = 1;
  }

}



void Orpsensors::run(Clara &clara) { // Has to be passed as a reference! See p. 255 in "Grundkurs C++"

  //if(clara.currentTime >= (clara.cloopTime + 1000)){

    update_measurement_values();
    pumpcount_new = clara.pump_count;
    update_conditions_to_change_pumptime();
    clara.set_lower_orp(lower_orp_curr);
    clara.set_upper_orp(upper_orp_curr);
    
    
    // debug
//    Serial.print("codition 1: ");
//    Serial.print(condition_1);
//    Serial.print("    condition 2: ");
//    Serial.print(condition_2);
//    Serial.print("    condition 3: ");
//    Serial.print(condition_3);
//    Serial.print("    pump Count: ");
//    Serial.print(clara.pump_count);
//    Serial.println("");
    

    if (condition_1 && condition_2 && condition_3){
      if (calculated_Cl_local > maximum_calculated_Cl){
        new_pumptime = clara.global_pump_time_total*0.8;
        }
      else if (calculated_Cl_local < minimum_calculated_Cl){
          new_pumptime = clara.global_pump_time_total*1.2;
        }
      // check if new pumptime is too big or too small and change pumptime
      if (new_pumptime < max_pumptime && new_pumptime > min_pumptime ){
          clara.global_pump_time_total = new_pumptime;
          condition_3 = 0;
      }
    //}
  }
  //delay(1000);
}
