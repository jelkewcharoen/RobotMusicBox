
//includes
#include <Servo.h>
#include <stdio.h>
#include <math.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal.h>

//tempo stuff
#define TempoCal 2000
#define TempoPotMax 1023
#define PwmMax 255

//Music Notes based on Octave--
#define C 16.3516
#define D 18.35405
#define E 20.60172
#define F 21.82676
#define G 24.49971
#define A 27.5
#define B 30.86771
#define high_C 32.70320
#define rest 0

//arduino inputs/outputs
#define speakerPIN 5
#define leftServo 10
#define rightServo 9
#define powerSwitch 7
#define modeSwitch 6
#define powerLED 4
#define tempoPIN A0
#define octavePIN A1
#define micPIN A2

//servo
#define servoRate 45

//state machine variables
enum {autoMode, manualMode};
unsigned char mode = manualMode;
enum {findTempo, findStart, start, playSong};
unsigned char action = findTempo;

// screen
const int rs = 13, en = 12, d4 = 11, d5 = 8, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Row Row Row Your Boat
int songLength = 54;  
int notes[] = {C, rest, C, rest, C, rest, D, rest, E, rest, E, rest, D, rest, E, rest, F, rest, G, rest, high_C, rest, high_C, rest, high_C, rest, G, rest, G, rest, G, rest, E, rest, E, rest, E, rest, C, rest, C, rest, C, rest, G, rest, F, rest, E, rest, D, rest, C, rest};
int beats[] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};

//global variables
double note_duration = 200, tempo = 5;
boolean mode_bool = false; //true is auto, false is manual
int octave = 5, octave_pot, tempo_pot, duration, i_note_index = 0;
int song_tempo = 250;

//servos
Servo servo_left;
Servo servo_right;
int servo_counter = 0;
bool servo_dir = false; //to keep track of the servos' direction


void setup() {
  //pin modes
  pinMode(tempoPIN, INPUT); //tempo
  pinMode(octavePIN, INPUT); //octave
  pinMode(micPIN, INPUT); //mic
  pinMode(modeSwitch, INPUT);

  //set up outputs
  pinMode(speakerPIN, OUTPUT);

  servo_left.attach(leftServo);
  servo_right.attach(rightServo);

  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  //Print a message to the LCD.
  lcd.print("Starting the Man");

  //debug only
  Serial.begin(115200);
  Serial.print("Setup Complete\n");

}

void loop() {
  printToScreen();
  switch(mode) {
    case autoMode:
      switch(action) {
        case findTempo:
          findAutoTempo();
          findOctave(octave);
          action = findStart;
          break;
        case findStart:
          //findStart();
          action = start;
          break;
        case start:
          action = playSong;
          break;
        case playSong:
          //play the song
          duration = beats[i_note_index] * tempo;
          
          tone(speakerPIN, notes[i_note_index]*pow(2,octave), duration);
          delay(duration);
            
          //increment the note counter
          ++i_note_index;
          if(i_note_index >= songLength) 
          {
            i_note_index = 0;
          }
          motors();
          break;
        
      }
      break;

    case manualMode:
      switch(action) {
        case findTempo:
          findManualTempo(tempo);
          findOctave(octave);
          action = playSong;
          break;
        case playSong:
          //play the song
          duration = beats[i_note_index] * note_duration;
          
          tone(speakerPIN, notes[i_note_index]*pow(2,octave), duration);
          delay(duration);
            
          //increment the note counter
          ++i_note_index;
          if(i_note_index >= songLength) 
          {
            i_note_index = 0;
          }
          motors();
          action = findTempo;
          break;
        
      }
      break;
    
  }
  printToScreen();
  //Serial.println(mode);
  //Serial.println(action);
  checkMode(mode_bool);
  if(mode_bool) {
    mode = autoMode;
  } else {
    mode = manualMode;
  }
}

void motors() {
  //set the servos 
  //Serial.println(servo_counter);
  if(servo_dir){
    servo_left.write(180-servo_counter);
    servo_right.write(servo_counter);
  }
  else{
    servo_left.write(servo_counter);
    servo_right.write(180-servo_counter);
  }

  servo_counter+=servoRate; 
  if(servo_counter>180){
    servo_counter = 0;
    servo_dir = !servo_dir; //flip direction
  }
}

void findAutoTempo() {
  
}

void findAutoStart() {
  
}

void printToScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if(mode_bool) 
    lcd.print("Auto  ");
  else 
    lcd.print("Manual");
  lcd.setCursor(0, 1);
  lcd.print("Tempo: ");
  lcd.setCursor(6, 1);
  lcd.print(tempo);
  lcd.setCursor(7, 0);
  lcd.print("Octave: ");
  lcd.setCursor(14, 0);
  lcd.print(octave);
}

void initializeThreshold() {
  
}

void findManualTempo(double &tempo) {
  //read the tempo pot
  int tempo_pot = analogRead(tempoPIN);
      
  note_duration = song_tempo*float(tempo_pot)/TempoCal; //read the tempo POT       
  //Serial.println(tempo);
//  if(note_duration<100){
//    note_duration = 100;
//  }
  tempo = 1000/note_duration;
}

void findOctave(int &octave) {
  octave_pot = analogRead(octavePIN);
  if(octave_pot > 511) {
      octave = 5;
  } else {
      octave = 4;
  }
}

void checkMode(boolean &mode_bool) {
  if(digitalRead(modeSwitch)) {
    mode_bool = true;
  } else {
    mode_bool = false;
  }
}
