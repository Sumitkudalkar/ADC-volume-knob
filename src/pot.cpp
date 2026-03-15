#include <Arduino.h>
#include <vector>
using namespace std;

#define POT         12
#define LED1        13
#define LED2        14
#define LED3        27
#define LED4        26
#define LED5        25
#define LED6        33
#define LED7        32
#define LED8        35
#define LED9        34
#define LED10       19
#define LED11       18
#define LED12       17



void setup(){
    Serial.begin(115200);
    pinMode(POT, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    pinMode(LED6, OUTPUT);
    pinMode(LED7, OUTPUT);
    pinMode(LED8, OUTPUT);
    pinMode(LED9, OUTPUT);
    pinMode(LED10, OUTPUT);
    pinMode(LED11, OUTPUT);
    pinMode(LED12, OUTPUT);
}

vector<uint32_t> ReadSensors(uint8_t pin){
    //taking raw data 
    vector<uint32_t> data(5, 0);
    for(int i=0; i<5; i++){
            uint32_t rawdata = analogRead(pin);
            data[i] = rawdata;
    }
    return data;
}

uint32_t FilterReadings(vector<uint32_t> &arr){ //call by ref such that change happens in data array itself
    //taking average
    uint32_t sum = 0;
    for(int i=0; i<5; i++){
        sum += arr[i];
    }
    return sum/5;
}

uint8_t ComputeReadings(uint32_t val){
    uint8_t LedToTurnON = (val)/341;
    return LedToTurnON;
}


void UpdateOutput(uint8_t num){
    uint8_t LED[12] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11, LED12};
    for(int i=0; i<12; i++){
        if(i<num){
            digitalWrite(LED[i], HIGH);
        }
        else{
            digitalWrite(LED[i], LOW);
        }
    }
}


void loop(){
    vector<uint32_t> sensorData = ReadSensors(POT);
    uint32_t filteredValue = FilterReadings(sensorData);
    uint8_t LEDs = ComputeReadings(filteredValue);
    UpdateOutput(LEDs);
}