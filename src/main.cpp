#include <Arduino.h>
// #include<WiFiManager.h>

// //gate is in idle state first -> opens -> closes -> goes back to idle

// enum class state : uint8_t{
//   IDLE,
//   OPEN,
//   CLOSE,
// };

// state currentstate = state::IDLE; //first idle

// void setup() {
// //   WiFi.mode(WIFI_STA); //set esp to station mode
// //   Serial.begin(9600);
// //   WiFiManager wm; //creating object wm

// //   //wm.resetSettings(); //resetting everything if any ip is there previously

// //   bool connect;
// //   connect = wm.autoConnect("espWifi", "password");

// //   if(connect){
// //     Serial.print("wifi connected");
// //   }
// //   else{
// //     Serial.print("no wifi detected");
// //   }
//   switch (currentstate)
//   {
//   case state::OPEN :
//     static uint32_t opentime = millis();
//     if(digitalRead())
//     break;
  
//   default:
//     break;
//   }
  
// }

// void loop() {
  

// }

// #define normalbtn     2
// #define vipbtn        3
// #define redled        4
// #define greenled      5


// enum class states : uint8_t{
//     IDLE,
//     NORMAL_OPEN,
//     VIP_OPEN
// };

// states currstate = states::IDLE;
// uint32_t normal_ontime = 0;
// uint32_t vip_ontime = 0;

// void setup() {
//   pinMode(normalbtn, INPUT_PULLUP);
//   pinMode(vipbtn, INPUT_PULLUP);
//   pinMode(redled, OUTPUT);
//   pinMode(greenled, OUTPUT);

// }

// void loop() {
//     switch (currstate)
//     {
//     case states::IDLE :
//       //for idle red led will be high and green low
//       digitalWrite(redled, HIGH);
//       digitalWrite(greenled, LOW);

//       if(digitalRead(normalbtn) == LOW){
//         normal_ontime = millis();
//         currstate = states::NORMAL_OPEN;
//       }
//       if(digitalRead(vipbtn) == LOW){
//         vip_ontime = millis();
//         currstate = states::VIP_OPEN;
//       }
//       break;
//     case states::NORMAL_OPEN:
//       //here normal car will go
//         digitalWrite(redled, LOW);
//         //green led will be high only for 3000ms
//         digitalWrite(greenled, HIGH);
//         if(millis() - normal_ontime > 3000){
//           digitalWrite(greenled, LOW);
//            //turning the green led low after 3sec
//           currstate = states::IDLE; //going to normal state
//         }
//       break;
//     case states::VIP_OPEN:
//         //vip car will go
//         digitalWrite(redled, LOW);
//         //green led will be high only for 5000ms
//         digitalWrite(greenled, HIGH);
//         if(millis() - vip_ontime > 5000){
//           digitalWrite(greenled, LOW);
//           //turning the green led low after 5sec
//            currstate = states::IDLE; //going to normal state
//         }
//       break;
      
    
//     default:
//         Serial.println("this case shoudnt occure : error 404");
//       break;
//     }
// }

#define CARSENSOR        2
#define TICKETBTN        3
#define STATUSLED        4
#define GATELED          5

void setup(){
  pinMode(CARSENSOR, INPUT_PULLUP);
  pinMode(TICKETBTN, INPUT_PULLUP);
  pinMode(STATUSLED, OUTPUT);
  pinMode(GATELED, OUTPUT);
}

enum class states : uint8_t{
    IDLE,
    WAITING,
    PRINTING,
    GATE_OPEN
};

states currentState = states::IDLE;
uint32_t wait_time = 0;
uint32_t print_time = 0;
uint32_t gate_time = 0;
uint32_t time_counting = 0;

void loop(){
  switch (currentState)
  {
  case states::IDLE :
  //turn off both the LEDs
  digitalWrite(STATUSLED, LOW);
  digitalWrite(GATELED, LOW);
  if(digitalRead(CARSENSOR) == LOW){
    //means car has been detected we go in wait state
    wait_time = millis();
    currentState = states::WAITING;
  }
    break;
  
  case states::WAITING :
  //wait for 5 sec if they dont press ticketbtn within 5 sec go to idle again
  digitalWrite(STATUSLED, HIGH); 

  if((millis() - wait_time < 5000) && (digitalRead(TICKETBTN) == LOW)){
    //means that ticketbtn is pressed before 5 seconds
    //we go in print mode
    currentState = states::PRINTING;
    print_time = millis();
    time_counting = millis();
  }
  else if(millis() - wait_time > 5000){ //means that 5 seconds has passed
    //we go in idle state 
    currentState = states::IDLE;
  }
  break;

  case states::PRINTING :
  //blink the led for 2 seconds
  if(millis() - print_time > 200){
    digitalWrite(STATUSLED, !digitalRead(STATUSLED));
    print_time = millis();
  }
  if(millis() - time_counting> 2000){
    //2seconds has been passed so now open the gate
    currentState = states::GATE_OPEN;
    gate_time = millis();
  }
  break;

  case states::GATE_OPEN :
  //turn off the status led 
  digitalWrite(STATUSLED, LOW);
  //on the gate led
  digitalWrite(GATELED, HIGH);

  //goto idle after 3sec
  if(millis() - gate_time > 3000){
    currentState = states::IDLE;
  }

  default:
  Serial.println("this wasnt suppose to happen, error 404");
    break;
  }
}