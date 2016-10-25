#include <IRremote.h>
#include <IRremoteInt.h>


/*
* IRremote: IRrecvDump - dump details of IR codes with IRrecv
* An IR detector/demodulator must be connected to the input RECV_PIN.
* Version 0.1 July, 2009
* Copyright 2009 Ken Shirriff
* http://arcfn.com
* JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
* LG added by Darryl Smith (based on the JVC protocol)
*/
 
int RECV_PIN = 11;
 
IRrecv irrecv(RECV_PIN);
 
decode_results results;
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
 
// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
// decode_results *results = (decode_results *)v
void dump(decode_results *results)
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  } 
  else if (results->decode_type == PANASONIC) { 
    Serial.print("Decoded PANASONIC - Address: ");
    //Serial.print(results->panasonicAddress,HEX);
    Serial.print(" Value: ");
  }
  //else if (results->decode_type == LG) {
    //Serial.print("Decoded LG: ");
  //}
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
 
  }
  //else if (results->decode_type == AIWA_RC_T501) {
    //Serial.print("Decoded AIWA RC T501: ");
  //}
  //else if (results->decode_type == WHYNTER) {
    //Serial.print("Decoded Whynter: ");
  //}
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
    Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.print("): ");
 
  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i], DEC);
    }
    else {
      Serial.print((int)results->rawbuf[i], DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}

//正解データ         
int switch1[23] = {25,8,25,8,8,25,25,8,25,8,8,25,9,25,8,25,8,25,8,25,8,25,25};  //切
int right[23]   = {25,8,25,8,9,24,26,8,25,8,9,25,9,25,9,25,9,25,26,8,9,25,9};   //リズム・お休み
int left[23]    = {26,8,25,8,9,25,25,8,26,8,9,25,9,25,9,25,25,8,9,25,9,25,9};   //タイマー
int mid[23]     = {25,8,25,8,9,25,26,8,26,8,9,25,9,25,25,8,9,24,9,25,9,25,9};   //首振り

bool isButton(decode_results *results, int *answer)
{
  for(int i = 1; i < results->rawlen; i++){//1つ目のデータは無視
    if( (results->rawbuf[i] > (answer[i-1]+2)) || ((answer[i-1]-2) > results->rawbuf[i]) ){
      return false;
    }
  }
  return true;
}

void loop() {
  bool swFlg = true;
  if (irrecv.decode(&results)) {
//    dump(&results);
    if (isButton(&results, right)){
      Serial.println("right");
    }else if(isButton(&results, left)){
      Serial.println("left");
    }else if(isButton(&results, mid)){
      Serial.println("mid");
    }else if(isButton(&results, switch1)){
      Serial.println("switch");
    }else{
      Serial.println("under");
    }
    irrecv.resume(); // Receive the next value
  }
}
