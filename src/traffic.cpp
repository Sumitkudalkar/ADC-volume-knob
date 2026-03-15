#include <Arduino.h>


#define REDled            2
#define YELLOWled         3
#define GREENled          4

#define interruptBTN      5
#define crosswalkREDled   6
#define crosswalkGREENled 7

void setup(){
    pinMode(REDled, OUTPUT);
    pinMode(YELLOWled, OUTPUT);
    pinMode(GREENled, OUTPUT);
    pinMode(interruptBTN, INPUT_PULLUP);
    pinMode(crosswalkREDled, OUTPUT);
    pinMode(crosswalkGREENled, OUTPUT);
}

enum class states : uint8_t{
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW,
    PEDESTRIAN_INTERRUPT
};

states currentstate = states::RED;
uint32_t red_time = 0;
uint32_t red_yellow_time;
uint32_t green_time;
uint32_t yellow_time;
uint32_t cross_green_signal_time;

bool isPressed = false;
void readinputs(){
    if(digitalRead(interruptBTN) == LOW){
        isPressed = true;
    }
}



void updateStatemachine(){
    switch(currentstate){
        case states::RED:
        if(millis() - red_time > 5000){
            currentstate = states::RED_YELLOW;
            red_yellow_time = millis();
        }
        break;

        case states::RED_YELLOW:
        if(millis() - red_yellow_time > 2000){
            currentstate = states::GREEN;
            green_time = millis();
        }
        break;

        case states::GREEN:
        if(millis() - green_time > 5000){

            currentstate = states::YELLOW;
            yellow_time = millis();
        }
        break;

        case states::YELLOW:
        if(millis() - yellow_time > 2000){
            if(isPressed){
                currentstate = states::PEDESTRIAN_INTERRUPT;
                cross_green_signal_time = millis();
                isPressed = false;
            }
            else{
                currentstate = states::RED;
                red_time = millis();
            }
        }
        break;

        case states::PEDESTRIAN_INTERRUPT:
        if(millis() - cross_green_signal_time > 5000){
        currentstate = states::RED;
        red_time = millis();
        }
        break;

    }
}

void updateoutputs(){
    switch (currentstate)
    {
    case states::RED :
    digitalWrite(REDled, HIGH);
    digitalWrite(YELLOWled,LOW);
    digitalWrite(GREENled, LOW);
    digitalWrite(crosswalkREDled,HIGH);
    digitalWrite(crosswalkGREENled, LOW);
    break;

    case states::RED_YELLOW :
    digitalWrite(REDled, HIGH);
    digitalWrite(YELLOWled,HIGH);
    digitalWrite(GREENled, LOW);
    digitalWrite(crosswalkREDled,HIGH);
    digitalWrite(crosswalkGREENled, LOW);
    break;

    case states::GREEN :
    digitalWrite(REDled, LOW);
    digitalWrite(YELLOWled,LOW);
    digitalWrite(GREENled, HIGH);
    digitalWrite(crosswalkREDled,HIGH);
    digitalWrite(crosswalkGREENled, LOW);
    break;

    case states::YELLOW :
    digitalWrite(REDled, LOW);
    digitalWrite(YELLOWled,HIGH);
    digitalWrite(GREENled, LOW);
    digitalWrite(crosswalkREDled,HIGH);
    digitalWrite(crosswalkGREENled, LOW);
    break;
    

    case states::PEDESTRIAN_INTERRUPT :
    digitalWrite(REDled, HIGH);
    digitalWrite(YELLOWled,LOW);
    digitalWrite(GREENled, LOW);
    digitalWrite(crosswalkREDled,LOW);
    digitalWrite(crosswalkGREENled, HIGH);
    
    break;

    }
}

void loop(){
    readinputs();
    updateStatemachine();
    updateoutputs();
}
