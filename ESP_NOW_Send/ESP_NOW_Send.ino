#include <WiFi.h>
#include <esp_now.h>

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

bool send_flag = true;

typedef struct struct_message { //Send massage
  bool foil;
  bool foil_button;
  bool left_chest;
  bool right_chest;
  bool belly;
  bool back;
} struct_message;

struct_message state;

uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x67, 0xEA, 0xD8}; // Recv ESP WIFI MAC

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
}

void init_state(){
  state.foil = false;
  state.foil_button = false;
  state.left_chest = false;
  state.right_chest = false;
  state.belly = false;
  state.back = false;
  }
  
void detect(){
  if (digitalRead(foil_button_pin) == HIGH){
        state.foil_button = true;
      }else{
        state.foil_button = false;
      }
      
  if(analogRead(left_chest_pin) > threshold){
        state.left_chest = true;
      }else{
        state.left_chest = false;
      }
      
  if(analogRead(right_chest_pin) > threshold){
        state.right_chest = true;
      }else{
        state.right_chest = false;
      }
      
  if(analogRead(belly_pin) > threshold){
        state.belly = true;
      }else{
        state.belly = false;
      }
  
  if(analogRead(back_pin) > threshold){
        state.back = true;
      }else{
        state.back = false;
      }
  }

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo;
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

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
  
  detect();
  
  if (send_flag) { //Send_Data_ESP_NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &state, sizeof(state));
    }
  
  
  delay(10);  
  }
