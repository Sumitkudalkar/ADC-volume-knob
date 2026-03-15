#include<Arduino.h>

#define BTN      2
#define LED      3

enum class states : uint8_t{
    IDLE,
    SINGLE_PRESS,
    DOUBLE_PRESS,
    LONG_PRESS
};

states currentstate = states::IDLE;

void setup(){
    pinMode(BTN, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
}

uint32_t presstime;
uint32_t double_presstime;
uint32_t long_presstime;

uint32_t store_time = 0;

bool prev_state = HIGH;
bool curr_state;

bool ispressed = false;

void checkstate(){
    curr_state = digitalRead(BTN);
    if(curr_state == LOW){
        presstime = millis(); // button has been pressed 
        prev_state = LOW;
    }
    if(curr_state == LOW && millis() - presstime > 2000){ //button is still low even after 2 secs
        currentstate = states::LONG_PRESS;
        prev_state = LOW;
    }
    if(curr_state = !prev_state){
        //btn went from low to high or high to low 
        double_presstime = millis(); // there is a chance for double press
    }
    if(curr_state == LOW && millis() - double_presstime > 300){
        //btn went low again within 300ms then its double press
        currentstate = states::DOUBLE_PRESS;
    }
    else{
        currentstate = states::SINGLE_PRESS; //meaning our belief was wrong
    }

}

void respond(){

}

void main(){
    checkstate();
    respond();
}