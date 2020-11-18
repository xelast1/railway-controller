//compatible with train controller/railway controller v4 and above


  //flash delays
  int cycleDelay = 1000; //cycle
  int signalFlashDelay = 1000; //signal
  int amberFlashDelay = 1000; //amber

  //pins
  const int rp = 2; //red
  const int ap = 3; //amber
  const int gp = 4; //green
  const int btStatePin = 5; //bt state pin
  const int motionPin = 6; //motion sensor

  //function variables
  bool afState = false; //amber flash state
  bool sfState = false; //signal flash state
  int thisCycle = 0; //current cycle stage

  //manual mode variables
  bool rs = false; //red state
  bool as = false; //amber state
  bool gs = false; //green state

  //misc
  int data; //serial data in
  int mode = 0; //toggle(0) / manual(1) mode
  unsigned long lastMillis = millis(); //millisecond timer
  bool btState = false; //whether bluetooth is connected
  int lastMotion = 0; //last time motion detected
  bool bypass = false; //used for running auto sequence
  int motionRange = 60000; //motion Tolerance
  bool useMotion = false //if motion detection is enabled
  
  
void setup() {
  Serial.begin(9600); //start serial (bt)

  //set pins
  pinMode(rp,OUTPUT);
  pinMode(ap,OUTPUT);
  pinMode(gp,OUTPUT);
  pinMode(btStatePin,INPUT);
  pinMode(motionPin,INPUT);

  wake();
}

void loop() {
  while(btState || bypass){ //if bluetooth connected or bypass triggered
    while(Serial.available() || bypass){ //if bt data received or bypass triggered
      !bypass ? data = Serial.read() : true; //if sequence not triggered by bypass, set data to bt data else keep data as preset value
      bypass = false; //disable bypass for next run
      splitter: //used to jump to start of switch ?? TODO - is there a better way to do this, gotos are discouraged
      switch(data){
        case 1: //red
        if(rs){ //if red on, turn off
          digitalWrite(rp,LOW);
          rs = false;
        }else{ //otherwise turn on red
          digitalWrite(rp,HIGH);
          rs = true;
          if(mode = 0){ //leave other pins as they were, unless auto enabled. if so, turn them off
            digitalWrite(ap,LOW);
            digitalWrite(gp,LOW);
            as = LOW;
            gs = LOW;
          }
        }
        break;
        case 2: //amber
        if(as){
          digitalWrite(ap,LOW);
          as = false;
        }else{
          digitalWrite(ap,HIGH);
          as = true;
          if(mode = 0){
            digitalWrite(rp,LOW);
            digitalWrite(gp,LOW);
            rs = LOW;
            gs = LOW;
          }
        }
        break;
        case 3: //green
        if(gs){
          digitalWrite(gp,LOW);
          gs = false;
        }else{
          digitalWrite(gp,HIGH);
          gs = true;
          if(mode = 0){
            digitalWrite(rp,LOW);
            digitalWrite(ap,LOW);
            rs = LOW;
            as = LOW;
          }
        }
        break;
        case 4: //all
          digitalWrite(rp, HIGH);
          digitalWrite(ap, HIGH);
          digitalWrite(gp, HIGH);
          rs = true;
          as = true;
          gs = true;
          break;  
          case 5: //cycle
          while(data == -1 || data == 5){
            if (millis() - lastMillis >= cycleDelay) {
               switch (thisCycle) {
                 case 0:
                   digitalWrite(gp, HIGH);
                   digitalWrite(ap, LOW);
                   digitalWrite(rp, LOW);
                   gs = true;
                   as = false;
                   rs = false;
                   thisCycle += 1;
                   break;
                 case 1:
                  digitalWrite(gp, LOW);
                  digitalWrite(ap, HIGH);
                  digitalWrite(rp, LOW);
                  gs = false;
                  as = true;
                  rs = false;
                  thisCycle += 1;
                  break;
                 case 2:
                    digitalWrite(gp, LOW);
                    digitalWrite(ap, LOW);
                    digitalWrite(rp, HIGH);
                    gs = false;
                    as = false;
                    rs = true;
                    thisCycle = 0;
                    break;
                }
                lastMillis = millis();
              }
              data = Serial.read();
              if (data != 5 && data != -1) {
                goto splitter;
              }
          }  
          break;
          case 6: //amber flash
          as = false;
          if (data == 6) {
            while (data == -1 || data == 6) {
              if (millis() - lastMillis >= amberFlashDelay) {
                if (as == false) {
                  digitalWrite(ap, LOW);
  
                  as = true;
                } else {
                  digitalWrite(ap, HIGH);
                  as = false;
                }
                lastMillis = millis();
              }
              data = Serial.read();
              if (data != 6 && data != -1) {
                goto splitter;
              }
            }
          }
          break;
          case 7: //off
          digitalWrite(rp, LOW);
          digitalWrite(ap, LOW);
          digitalWrite(gp, LOW);
          rs = false;
          as = false;
          gs = false;
          break;  
          case 8: //signal flash
          if (data == 8) {
            while (data == -1 || data == 8) {
              if (millis() - lastMillis >= signalFlashDelay) {
                if (sfState == false) {
                  digitalWrite(rp, LOW);
                  digitalWrite(ap, LOW);
                  digitalWrite(gp, LOW);
                  rs = false;
                  as = false;
                  gs = false;
                  
                  sfState = true;
                } else {
                  digitalWrite(rp, HIGH);
                  digitalWrite(ap, HIGH);
                  digitalWrite(gp, HIGH);
                  sfState = false;
                  rs = true;
                  as = true;
                  gs = true;
                }
                lastMillis = millis();
              }
              data = Serial.read();
              if (data != 8 && data != -1) {
                goto splitter;
              }
            }
          }
         break;
         case 9: //toggle mode
         mode = 0;
         setNull();
         break;
         case 10: //manual mode
         mode = 1;
         setNull();
         break;
         case 11: //bounce
         while(data == -1 || data == 11){
            if (millis() - lastMillis >= cycleDelay) {
               switch (thisCycle) {
                 case 0:
                   digitalWrite(gp, HIGH);
                   digitalWrite(ap, LOW);
                   digitalWrite(rp, LOW);
                   rs = false;
                   as = false;
                   gs = true;
                   thisCycle += 1;
                   break;
                 case 1:
                  digitalWrite(gp, LOW);
                  digitalWrite(ap, HIGH);
                  digitalWrite(rp, LOW);
                  rs = false;
                   as = true;
                   gs = false;
                  thisCycle += 1;
                  break;
                 case 2:
                    digitalWrite(gp, LOW);
                    digitalWrite(ap, LOW);
                    digitalWrite(rp, HIGH);
                    rs = true;
                   as = false;
                   gs = false;
                    thisCycle += 1;
                    break;
                case 3:
                digitalWrite(gp, LOW);
                digitalWrite(ap, HIGH);
                digitalWrite(rp, LOW);
                rs = false;
                   as = true;
                   gs = false;
                thisCycle = 0;
                break;
                }
                lastMillis = millis();
              }
              data = Serial.read();
              if (data != 11 && data != -1) {
                goto splitter;
              }
          }
         break;
         case 12: //stack
         while(data == -1 || data == 12){
            if (millis() - lastMillis >= cycleDelay) {
               switch (thisCycle) {
                 case 0:
                   digitalWrite(gp, HIGH);
                   digitalWrite(ap, HIGH);
                   digitalWrite(rp, HIGH);
                   rs = true;
                   as = true;
                   gs = true;
                   thisCycle += 1;
                   break;
                 case 1:
                  digitalWrite(gp, LOW);
                  digitalWrite(ap, HIGH);
                  digitalWrite(rp, HIGH);
                  rs = true;
                   as = true;
                   gs = false;
                  thisCycle += 1;
                  break;
                 case 2:
                    digitalWrite(gp, LOW);
                    digitalWrite(ap, LOW);
                    digitalWrite(rp, HIGH);
                    rs = true;
                   as = false;
                   gs = false;
                    thisCycle += 1;
                    break;
                case 3:
                digitalWrite(gp, LOW);
                digitalWrite(ap, LOW);
                digitalWrite(rp, LOW);
                rs = false;
                   as = false;
                   gs = false;
                thisCycle += 1;
                break;
                case 4:
                digitalWrite(gp, LOW);
                digitalWrite(ap, LOW);
                digitalWrite(rp, HIGH);
                rs = true;
                   as = false;
                   gs = false;
                thisCycle += 1;
                break;
                case 5:
                digitalWrite(gp, LOW);
                digitalWrite(ap, HIGH);
                digitalWrite(rp, HIGH);
                rs = true;
                   as = true;
                   gs = false;
                thisCycle += 1;
                break;
                case 6:
                digitalWrite(gp, HIGH);
                digitalWrite(ap, HIGH);
                digitalWrite(rp, HIGH);
                rs = true;
                   as = true;
                   gs = true;
                thisCycle += 1;
                break;
                case 7:
                digitalWrite(gp, HIGH);
                digitalWrite(ap, HIGH);
                digitalWrite(rp, LOW);
                rs = false;
                   as = true;
                   gs = true;
                thisCycle += 1;
                break;
                case 8:
                digitalWrite(gp, HIGH);
                digitalWrite(ap, LOW);
                digitalWrite(rp, LOW);
                rs = false;
                   as = false;
                   gs = true;
                thisCycle += 1;
                break;
                case 9:
                digitalWrite(gp, LOW);
                digitalWrite(ap, LOW);
                digitalWrite(rp, LOW);
                rs = false;
                   as = false;
                   gs = false;
                thisCycle += 1;
                break;
                case 10:
                digitalWrite(gp, HIGH);
                digitalWrite(ap, LOW);
                digitalWrite(rp, LOW);
                rs = false;
                   as = false;
                   gs = true;
                thisCycle += 1;
                break;
                case 11:
                digitalWrite(gp, HIGH);
                digitalWrite(ap, HIGH);
                digitalWrite(rp, LOW);
                rs = false;
                   as = true;
                   gs = true;
                thisCycle = 0;
                break;
                }
                lastMillis = millis();
              }
              data = Serial.read();
              if (data != 12 && data != -1) {
                goto splitter;
              }
          }
         break;
      }
      
    }
    digitalRead(btStatePin) == HIGH ? btState = true : btState = false;
    isLive();
  }
  setNull();
  wake();
}
void setNull(){ //set all pins to off
  digitalWrite(rp,LOW);
  digitalWrite(ap,LOW);
  digitalWrite(gp,LOW);
  sfState = false;
  afState = false;
  thisCycle = 0;
  rs = false;
  as = false;
  gs = false;
}
void wake(){ //wake sequence
    while(!btState){ //while bt not paired  
      if(digitalRead(motionPin) == HIGH && useMotion == true){ //motion detected, triggers wake
        lastMotion = millis();
      }
      if(millis() - lastMotion < motionRange){
        if(millis() - lastMillis <= 10000){ //every 10 seconds perform random action
          data = random(1,13);
          bypass = true;
          loop();
        }
      }
      digitalRead(btStatePin) == HIGH ? btState = true : btState = false; //check if bt connected
  }
  setNull();
  loop();
}
void isLive(){ //check if motion detected
  if(digitalRead(motionPin) == HIGH && useMotion == true){
    lastMotion = millis();
  }
  if(!btState){
    if(useMotion == true){
      if(millis() - lastMotion >= motionRange){ //if no motion for more than 10 minutes
        setNull();
        wake();
      }
    }else{
      setNull();
      wake();
    }
  }
}
