/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int leds[] = {13, 12, 11, 10, 9};

unsigned char qntSeq1 = 1;
unsigned char qntSeq2 = 0;
unsigned char qntSeq3 = 0;
unsigned char qntSeq4 = 0;

int sequencia = 1;

// the setup routine runs once when you press reset:
void setup() {  
  Serial.begin(9600); 
  
  // initialize the digital pin as an output.
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(9, OUTPUT); 
}

// the loop routine runs over and over again forever:
void loop() {
  char inByte; 
  int i;
  
  if (Serial.available()) {
    
    inByte = Serial.read();
    
    if(inByte == '1'){
      if (sequencia < 4){
        sequencia++;
        
        if(sequencia == 2)
          qntSeq2++;
        if(sequencia == 3)
          qntSeq3++;
        if(sequencia == 4)
          qntSeq4++;
      }
    }
    
    if(inByte == '2'){
      if (sequencia > 1){
        sequencia--;
        
        if(sequencia == 1)
          qntSeq1++;
        if(sequencia == 2)
          qntSeq2++;
        if(sequencia == 3)
          qntSeq3++;
      }
    }
    
    if(inByte == '3'){
      Serial.write(qntSeq1);
      Serial.write(qntSeq2);
      Serial.write(qntSeq3);
      Serial.write(qntSeq4);
    }
    
    Serial.write(inByte);
  }
    
  if(sequencia == 1){
    for(i = 0; i < 5; i++){
      digitalWrite(leds[i], HIGH);
      delay(250);
      digitalWrite(leds[i], LOW);
      delay(250);
    }
  }
  
  if(sequencia == 2){
    for(i = 0; i < 5; i++){
      digitalWrite(leds[i], HIGH);
    }
    delay(250);
    
    for(i = 0; i < 5; i++){
      digitalWrite(leds[i], LOW);
    }
    delay(250);
  }
  
  if(sequencia == 3){
    for(i = 0; i < 5; i++){
      digitalWrite(leds[i], HIGH);
    }
    delay(1000);
    
    for(i = 0; i < 5; i = i + 1){
      digitalWrite(leds[i], LOW);
      delay(500);
    }
  }
  
  if(sequencia == 4){
    digitalWrite(leds[0], HIGH);
    delay(500);
    digitalWrite(leds[4], HIGH);
    delay(500);
    digitalWrite(leds[1], HIGH);
    delay(500);
    digitalWrite(leds[3], HIGH);
    delay(500);
    digitalWrite(leds[2], HIGH);
    delay(500);
     
    
    digitalWrite(leds[0],LOW);
    delay(500);
    digitalWrite(leds[4], LOW);
    delay(500);
    digitalWrite(leds[1], LOW);
    delay(500);
    digitalWrite(leds[3], LOW);
    delay(500);
    digitalWrite(leds[2], LOW);
    delay(500);
  }
  
}
