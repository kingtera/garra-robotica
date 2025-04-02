#include <Arduino.h>
#include <Servo.h>

//SERVOS
Servo garra_esquerda;
Servo garra_direita;
Servo base;
Servo junta;

//CONSTANTS
#define begin_degree 90
#define end_degree   180

//STATUS VARIABLES
int close_status = 0;
int open_status  = 0;
int first_position_status = 0;
int last_position_status  = 0;
int up_status   = 0;
int down_status = 0;
int catch_status = 0;

//FUNCTIONS
void open_grab();
void close_grab();
void move_base();
void move_junta();

void setup() {
  garra_esquerda.attach(7);
  garra_direita.attach(6);
  base.attach(5);
  junta.attach(4);

  base.write(begin_degree);

  Serial.begin(9600);
}

void loop() {
  go_work();
  delay(1000);
}


//FUNCTIONS DESCRIPTION
void go_work(){
  junta.write(90);

  if(base.read() == 90) {
    first_position_status = 1;
    last_position_status = 0;
  }

  delay(1500);

  if(abs(junta.read() - 90) < 5) {
    up_status = 1;
    down_status = 0;
  }

  if(up_status) { 
    Serial.println("Descendo...\n");

    for(int i = 90; i >= 0; i--){
      junta.write(i);
      i = i - 9;
      delay(500);
    }

    up_status = 0;
    down_status = 1;
  }

  //CATCH
  if(down_status && !(catch_status)) {
    close_grab();
    catch_status = 1;
    delay(800);
  }
  else if(down_status && catch_status) {
    open_grab();
    catch_status = 0;
    delay(800);
  }

  if(down_status) { 
    Serial.println("Subindo...\n");

    for(int i = 0; i <= 90; i++){
      junta.write(i);
      i = i + 9;
      delay(500);
    }
  }
  
  if(base.read() == 180) {
    first_position_status = 0;
    last_position_status = 1;
  }
  delay(1500);

  move_base();
}

void move_base() {
  if(first_position_status) {
    Serial.println("Indo...\n");
    
    for(int i = begin_degree; i <= end_degree; i++){
      base.write(i);
      delay(100);
    }
  }

  if(last_position_status) {
    Serial.println("Voltando...\n");

    for(int i = end_degree; i >= begin_degree; i--){
      base.write(i);
      delay(100);
    }
  }
}

void open_grab() {
  Serial.println("Open\n");
  garra_esquerda.write(180);
  garra_direita.write(0);

  if((garra_esquerda.read() == 180) && (garra_direita.read() == 0)) {
    open_status = 1;
    close_status = 0;
  }
}

void close_grab() {
  Serial.println("Close\n");
  garra_esquerda.write(90);
  garra_direita.write(90);

  if((garra_esquerda.read() == 90) && (garra_direita.read() == 90)) {
    open_status = 0;
    close_status = 1;
  }
}