#ifndef MY_CLARA_H
#define MY_CLARA_H
#include "Arduino.h"



using namespace std;

class Clara {

  private:
    byte state = 1;

    double flow;
    byte   eflow;
    float batterypercentage;



  public:
    // init-methods
    Clara();
    void init();

    int           liquidlevel_1 = 0;
    int           liquidlevel_2 = 0;
    int           liquidlevel_3 = 0;
    int           upper_orp;
    int           lower_orp;
    float         calculated_Cl;
    float         volume_per_pump = 0.005;
    int           pump_count = 0;
    int           four_minutes_pump_count =0;
    int            wa_state;
    int            v_state;
    float        voltage ;
    long          watamin = 120000;
    int           pin;
    int          timer = 0;
    boolean     wata_ready = 0;
    boolean     ready_to_settle = 0;
    int         count_down_min = 180; 
    float       flow_commulative = 0;
    int       global_pump_time_total;
    float     commulative_pump_time;


    // state-methods





    // flow-methods
    // flow in [l/min] in here
    double get_flow();
    void set_flow(double flow);
    int get_state();
    void set_state(int state);
    byte  get_eflow();
    void set_eflow(byte eflow);

    


    
    
    void set_liquidlevel(int liquidlevel);
    int  get_liquidlevel();
    float  get_voltage();
    void  set_voltage(float voltage);
    float get_batterypercentage();
    void set_batterypercentage(float batterypercentage);
    int     get_liquidlevel_1();
    void  set_liquidlevel_1(int liquidlevel_1);
    int     get_liquidlevel_2();
    void  set_liquidlevel_2(int liquidlevel_2);
    int get_liquidlevel_3();
    void set_liquidlevel_3(int liquidlevel_3);
    int get_upper_orp();
    void set_upper_orp(int upper_orp);
    int get_lower_orp();
    void set_lower_orp(int lower_orp);

    void Start_wata(int wa_state, int pin);
    void  Stop_wata(int wa_state, int pin);
    void  Open_Valve(int v_state, int pin);
    void  Close_Valve(int v_state, int pin);
    


};
#endif
