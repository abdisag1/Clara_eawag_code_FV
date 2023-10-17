#ifndef MY_DISPLAY_H
#define MY_DISPLAY_H
#include <Arduino.h>
#include "Clara.h"
#include <LiquidCrystal_I2C.h>




using namespace std;

class Display {

  private:
    //LiquidCrystal_I2C lcd;
    byte pin;
    unsigned long currentTime;
    unsigned long cloopTime;
    String text;
    bool render_necessary(Clara &clara);
    void LCDWrite(String text1, String text2, String text3, String text4, LiquidCrystal_I2C &lcd);
    int last_mark_index;
    //int watamin=7200000;
    //static void tdCallback();
  public:
    Display(byte pin);
    void init();
    void run(Clara &clara,LiquidCrystal_I2C &lcd); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"

};
#endif
