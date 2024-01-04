#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

// touch constants
const uint8_t MPR121_ADDR = 0x5A;
const uint8_t MPR121_INT = 2;

int Lights;
int Order[7];
int x;

void setup() {
  Serial.begin(9600);

  MPR121.begin(MPR121_ADDR);
  MPR121.setInterruptPin(MPR121_INT);

  MPR121.setTouchThreshold(0,8);  // this is the touch threshold - setting it low makes it more like a proximity trigger, default value is 40 for touch
  MPR121.setReleaseThreshold(0,3);  // this is the release threshold - must ALWAYS be smaller than the touch threshold, default value is 20 for touch

  MPR121.setTouchThreshold(1,10);  
  MPR121.setReleaseThreshold(1,2);

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);
  MPR121.autoSetElectrodes();

  randomSeed(analogRead(A0));
  
  for(int i = 3; i < 10; i++){
    pinMode(i, OUTPUT);
  }
  for(int i = 3; i < 10; i++){
    digitalWrite(i, LOW);
  }
  for(int i = 0; i < 6; i++){
    digitalWrite(8-i, HIGH);
    delay(50);
  }
  delay(250);
  Off();
}

void loop() {
  Restart:
  MPR121.updateAll();

  for (int i = 0; i < 2; i++) {
    if (MPR121.isNewTouch(i)) {
      if(i == 0){
        Off();
        Lights = random(1,6);
        switch (Lights){
          case 1:
            for(int ii = 3; ii < 9; ii++){
              digitalWrite(ii, HIGH);
              delay(500);
              MPR121.updateAll();
              if(MPR121.isNewRelease(0)){
                Off();
                goto Restart;
              }
            }
            break;
          case 2:
            for(int ii = 8; ii > 2; ii--){
              digitalWrite(ii, HIGH);
              delay(500);
              MPR121.updateAll();
              if(MPR121.isNewRelease(0)){
                Off();
                goto Restart;
              }
            }
            break;
          case 3:
            for(int ii = 0; ii < 7; ii++){
              Order[ii] = 0;
            }
            for(int ii = 0; ii < 6; ii++){
              Repick:
              x = random(0,6);
              if(Order[x] == 0){
                Order[x] = 1;
                digitalWrite(x+3, HIGH);
                delay(500);
                MPR121.updateAll();
                if(MPR121.isNewRelease(0)){
                  Off();
                  goto Restart;
                }
              }
              else{
                goto Repick;
              }
            }
            break;
          case 4:
            for(int ii = 3; ii < 9; ii = ii +2){
              digitalWrite(ii, HIGH);
              digitalWrite(ii+1, HIGH);
              delay(1000);
              MPR121.updateAll();
              if(MPR121.isNewRelease(0)){
                Off();
                goto Restart;
              }
            }
            break;
          case 5:
            for(int ii = 8; ii > 2; ii = ii -2){
              digitalWrite(ii, HIGH);
              digitalWrite(ii-1, HIGH);
              delay(1000);
              MPR121.updateAll();
              if(MPR121.isNewRelease(0)){
                Off();
                goto Restart;
              }
            }
            break;
        }
        delay(1000);
        MPR121.updateAll();
        if(MPR121.isNewRelease(0)){
          Off();
          goto Restart;
        }
        
        for(int ii = 0; ii < 7; ii++){
          Order[ii] = 0;
        }
        for(int ii = 0; ii < 5; ii++){
          Repick2:
          x = random(0,6);
          if(Order[x] == 0){
            Order[x] = 1;
            digitalWrite(x+3, LOW);
            delay(500);
            MPR121.updateAll();
            if(MPR121.isNewRelease(0) && ii != 4){
              Off();
              goto Restart;
            }
          }
          else{
            goto Repick2;
          }
        }
      }
      else if(i == 1){
        Off();
        for(int iv = 0; iv < 6; iv++){
          for(int iii = 0; iii < 4; iii++){
            digitalWrite(8-iv, HIGH);
            delay(250-(iv*40));
            MPR121.updateAll();
            if(MPR121.isNewRelease(1)){
              Off();
              goto Restart;
            }
            digitalWrite(8-iv, LOW);
            delay(250-(iv*40));
            MPR121.updateAll();
            if(MPR121.isNewRelease(1)){
              Off();
              goto Restart;
            }
          }
          digitalWrite(8-iv, HIGH);
        }
        digitalWrite(9, HIGH);
        for(int v = 0; v < 40; v++){
          Off();
          delay(100);
          digitalWrite(3, HIGH);
          digitalWrite(4, HIGH);
          digitalWrite(5, HIGH);
          digitalWrite(6, HIGH);
          digitalWrite(7, HIGH);
          digitalWrite(8, HIGH);
          delay(100);
        }
        digitalWrite(9,LOW);
        Off();
      }
    }
  }
}

void Off(){
  for(int i = 3; i < 9; i++){
    digitalWrite(i, LOW);
  }
}
