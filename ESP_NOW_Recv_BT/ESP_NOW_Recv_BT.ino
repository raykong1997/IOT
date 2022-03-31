#include <WiFi.h>
#include <esp_now.h>

#include "BluetoothSerial.h"
#include "esp_bt_device.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define foil_button_pin     39
#define foil_pin            5
#define left_chest_pin      33
#define right_chest_pin     33
#define belly_pin           33
#define back_pin            33

#define PWM1_Ch             0
#define PWM1_Res            8
#define PWM1_DutyCycle      128
#define PWM1_Freq           5000

const int threshold = 20;
const int max_timeout = 50;
int timeout = 0;

bool send_flag = true;

BluetoothSerial SerialBT; //BT_use
bool bt_flag = false;
const int message_size = 13;
char message[message_size] = {'0','0','0','0','0','0','0','0','0','0','0','0','0'};

typedef struct struct_message { //Send massage
  bool foil;
  bool foil_button;
  bool left_chest;
  bool right_chest;
  bool belly;
  bool back;
} struct_message;

struct_message state,this_state;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&state, incomingData, sizeof(state));
  timeout = 0;
}

void write_message_to_bt() {
   
   if (state.foil == false)             {message[0] = '0';}else{message[0] = '1';}
   if (state.foil_button == false)      {message[1] = '0';}else{message[1] = '1';}
   if (state.left_chest == false)       {message[2] = '0';}else{message[2] = '1';}
   if (state.right_chest == false)      {message[3] = '0';}else{message[3] = '1';}
   if (state.belly == false)            {message[4] = '0';}else{message[4] = '1';}
   if (state.back == false)             {message[5] = '0';}else{message[5] = '1';}

   if (this_state.foil == false)        {message[6] = '0';}else{message[6] = '1';}
   if (this_state.foil_button == false) {message[7] = '0';}else{message[7] = '1';}
   if (this_state.left_chest == false)  {message[8] = '0';}else{message[8] = '1';}
   if (this_state.right_chest == false) {message[9] = '0';}else{message[9] = '1';}
   if (this_state.belly == false)       {message[10] = '0';}else{message[10] = '1';}
   if (this_state.back == false)        {message[11] = '0';}else{message[11] = '1';}

   if(timeout < max_timeout)           {message[12] = '1';}else{message[12] = '0'; timeout = max_timeout;}
   timeout++;
   SerialBT.print(message);
}

void init_state(){
  
  state.foil = false;                  //Set all state to false
  state.foil_button = false;
  state.left_chest = false;
  state.right_chest = false;
  state.belly = false;
  state.back = false;
  
  this_state.foil = false;
  this_state.foil_button = false;
  this_state.left_chest = false;
  this_state.right_chest = false;
  this_state.belly = false;
  this_state.back = false;
  }

void detect(){
  if (digitalRead(foil_button_pin) == HIGH){
        this_state.foil_button = true;
      }else{
        this_state.foil_button = false;
      }
      
  if(analogRead(left_chest_pin) > threshold){
        this_state.left_chest = true;
      }else{
        this_state.left_chest = false;
      }
      
  if(analogRead(right_chest_pin) > threshold){
        this_state.right_chest = true;
      }else{
        this_state.right_chest = false;
      }
      
  if(analogRead(belly_pin) > threshold){
        this_state.belly = true;
      }else{
        this_state.belly = false;
      }
  
  if(analogRead(back_pin) > threshold){
        this_state.back = true;
      }else{
        this_state.back = false;
      }
  }


void setup() {
  //Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
    
  esp_now_register_recv_cb(OnDataRecv);

  SerialBT.begin("ESP32_IOT");
  //Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(foil_button_pin, INPUT);                    //Input_Setup
  pinMode(left_chest_pin, INPUT);
  pinMode(right_chest_pin, INPUT);
  pinMode(belly_pin, INPUT);
  pinMode(back_pin, INPUT);

  ledcAttachPin(foil_pin, PWM1_Ch);              //PWM_foil_Setup
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
  ledcWrite(PWM1_Ch, PWM1_DutyCycle);

  init_state();
}

void loop() {
  
  if (SerialBT.connected()) {
    bt_flag = true;
  }else {
    bt_flag = false;
  }
  
  detect();
  
  if(bt_flag && send_flag) {
    write_message_to_bt();
  }
  delay(10);
}
