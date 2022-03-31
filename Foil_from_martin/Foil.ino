const int foilPin = 36;
const int foilButton = 39;
const int MARK_Pin = 26; //GOT_MARKS_LED_Pin OR RED PIN:27
const int NO_MARK_PIN = 25; //NOT VALID AREA


int threshold = 20;
bool touch1detected = false;
bool touch2detected = false;
bool touch3detected = false;
bool touch4detected = false;

int foilState = 0; //0
int ButtonState = 0;


uint32_t last_time = 0;
int detect_count = 0;
void gotTouch1()
{
 touch1detected = true;
}

void gotTouch2()
{
 touch2detected = true;
}

void gotTouch3()
{
 touch3detected = true;
}

void gotTouch4()
{
 touch4detected = true;
}


void setup() 
{
  Serial.begin(115200);  
  pinMode(foilPin, INPUT);
  pinMode(foilButton, INPUT);
  pinMode(MARK_Pin, OUTPUT);
  pinMode(NO_MARK_PIN, OUTPUT);

  touchAttachInterrupt(T6, gotTouch1, threshold);
  touchAttachInterrupt(T5, gotTouch2, threshold);
  touchAttachInterrupt(T4, gotTouch3, threshold);
  touchAttachInterrupt(T3, gotTouch4, threshold);
}


void loop() 
{

  if(last_time < millis())
  {
     digitalWrite(MARK_Pin, LOW);
     digitalWrite(NO_MARK_PIN, LOW);
    last_time = last_time+20;
    body_detect();
    ButtonState = digitalRead(foilButton); //攻擊部分
    if (ButtonState == HIGH)
    {
      foil_detect();
      } 
      else 
      {
        Serial.println("等待你打中人");
        }
        }
}

void foil_detect() //攻擊部分
{
  foilState = digitalRead(foilPin);
  Serial.print(foilState);
  Serial.print(",");

  if (foilState != LOW) 
  {
    Serial.println("打中人了");
    digitalWrite(MARK_Pin, HIGH);
     //send data
  } else {
    Serial.println(" 打唔中");
    digitalWrite(NO_MARK_PIN, HIGH);
     //send data
  }
}



void body_detect() //被攻擊部分
{
  if(touch1detected){
    touch1detected = false;
   Serial.println("左胸被擊中!");
    //send data
  }
  if(touch2detected){
    touch2detected = false;
   Serial.println("右胸被擊中!");
    //send data
  }
    if(touch3detected){
    touch3detected = false;
   Serial.println("腹肌被擊中");
    //send data
  }
  if(touch4detected){
    touch4detected = false;
   Serial.println("背部或側面被擊中");
    //send data
  }

}
