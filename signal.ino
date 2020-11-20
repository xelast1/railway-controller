//CURRENT VERSION v3.2.5

const int cd = 1000; //cycle delay
const int fd = 1000; //flash delay

const int rp = 2; //red pin
const int ap = 3; //amber pin
const int gp = 4; //green pin

int data; //serial read in
unsigned long lastMillis = millis(); //flash timer
int thisCycle = 0; //cycle timer
int as = false; //flash loop

void setup() {
  pinMode(rp, OUTPUT); //initilize red pin
  pinMode(ap, OUTPUT); //initilize amber pin
  pinMode(gp, OUTPUT); //initilize green pin
  digitalWrite(rp, LOW); //turn off all pins
  digitalWrite(ap, LOW);
  digitalWrite(gp, LOW);
  Serial.begin(9600); //for bluetooth communication
}
void loop() {
  while (Serial.available()) {
    data = Serial.read();
    switch (data) {
      case 1: //red
        digitalWrite(rp, HIGH);
        digitalWrite(ap, LOW);
        digitalWrite(gp, LOW);
        break;
      case 2: //amber
        digitalWrite(rp, LOW);
        digitalWrite(ap, HIGH);
        digitalWrite(gp, LOW);
        break;
      case 3: //green
        digitalWrite(rp, LOW);
        digitalWrite(ap, LOW);
        digitalWrite(gp, HIGH);
        break;
      case 4: //all
        digitalWrite(rp, HIGH);
        digitalWrite(ap, HIGH);
        digitalWrite(gp, HIGH);
        break;
      case 5: //cycle
        if (data == 5) {
          while (data == -1 || data == 5) {
            if (millis() - lastMillis >= cd) {
              switch (thisCycle) {
                case 0:
                  digitalWrite(gp, HIGH);
                  digitalWrite(ap, LOW);
                  digitalWrite(rp, LOW);
                  thisCycle += 1;
                  break;
                case 1:
                  digitalWrite(gp, LOW);
                  digitalWrite(ap, HIGH);
                  digitalWrite(rp, LOW);
                  thisCycle += 1;
                  break;
                case 2:
                  digitalWrite(gp, LOW);
                  digitalWrite(ap, LOW);
                  digitalWrite(rp, HIGH);
                  thisCycle = 0;
                  break;
              }
              lastMillis = millis();
            }
            data = Serial.read();
            if (data != 5 || data != -1) {
              loopBreak();
            }
          }
        }
        break;
      case 6: //amber flash
        digitalWrite(gp, LOW);
        digitalWrite(rp, LOW);
        if (data == 6) {
          while (data == -1 || data == 6) {
            if (millis() - lastMillis >= fd) {
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
            if (data != 6 || data != -1) {
              loopBreak();
            }
          }
        }
        break;
      case 7: //off
        digitalWrite(rp, LOW);
        digitalWrite(ap, LOW);
        digitalWrite(gp, LOW);
        break;
    }
  }
}

void loopBreak() {
  switch (data) {
    case 1: //red
      digitalWrite(rp, HIGH);
      digitalWrite(ap, LOW);
      digitalWrite(gp, LOW);
      break;
    case 2: //amber
      digitalWrite(rp, LOW);
      digitalWrite(ap, HIGH);
      digitalWrite(gp, LOW);
      break;
    case 3: //green
      digitalWrite(rp, LOW);
      digitalWrite(ap, LOW);
      digitalWrite(gp, HIGH);
      break;
    case 4: //all
      digitalWrite(rp, HIGH);
      digitalWrite(ap, HIGH);
      digitalWrite(gp, HIGH);
      break;
    case 5: //cycle
      if (data == 5) {
        while (data == -1 || data == 5) {
          if (millis() - lastMillis >= cd) {
            switch (thisCycle) {
              case 0:
                digitalWrite(gp, HIGH);
                digitalWrite(ap, LOW);
                digitalWrite(rp, LOW);
                thisCycle += 1;
                break;
              case 1:
                digitalWrite(gp, LOW);
                digitalWrite(ap, HIGH);
                digitalWrite(rp, LOW);
                thisCycle += 1;
                break;
              case 2:
                digitalWrite(gp, LOW);
                digitalWrite(ap, LOW);
                digitalWrite(rp, HIGH);
                thisCycle = 0;
                break;
            }
            lastMillis = millis();
          }
          data = Serial.read();
          if (data != 5 || data != -1) {
            loopBreak();
          }
        }
      }
      break;
    case 6: //amber flash
      digitalWrite(gp, LOW);
      digitalWrite(rp, LOW);
      if (data == 6) {
        while (data == -1 || data == 6) {
          if (millis() - lastMillis >= fd) {
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
          if (data != 6 || data != -1) {
            loopBreak();
          }
        }
      }
      break;
    case 7: //off
      digitalWrite(rp, LOW);
      digitalWrite(ap, LOW);
      digitalWrite(gp, LOW);
      break;
  }
}
