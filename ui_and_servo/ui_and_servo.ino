
#include <Servo.h>
#include <stdio.h>
#include <math.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal.h>

#define TempoCal 1000
#define TempoPotMax 1023
#define PwmMax 255

//Music Notes based on Octive--
#define C 16.3516
#define D 18.35405
#define E 20.60172
#define F 21.82676
#define G 24.49971
#define A 27.5
#define B 30.86771
#define high_C 32.70320
#define rest 0


#define speakerPIN 5
#define leftServo 10
#define rightServo 9
#define powerSwitch 7
#define modeSwitch 6
#define powerLED 4
#define tempoPIN A0
#define octavePIN A1

#define servoRate 45
#define clockwise 80
#define c_clockwise 100

//set up screen
const int rs = 13, en = 12, d4 = 11, d5 = 8, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Row Row Row Your Boat
int songLength = 54;  
int notes[] = {C, rest, C, rest, C, rest, D, rest, E, rest, E, rest, D, rest, E, rest, F, rest, G, rest, high_C, rest, high_C, rest, high_C, rest, G, rest, G, rest, G, rest, E, rest, E, rest, E, rest, C, rest, C, rest, C, rest, G, rest, F, rest, E, rest, D, rest, C, rest};
int beats[] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};

int song_tempo = 250;

//setup the servo output
Servo servo_left;
Servo servo_right;
int servo_counter = 0;

void setup()
{
  pinMode(tempoPIN, INPUT); //tempo
  pinMode(octavePIN, INPUT); //octave
  pinMode(powerSwitch, INPUT);
  pinMode(modeSwitch, INPUT);
  
    //set up outputs
  pinMode(speakerPIN, OUTPUT);
  pinMode(powerLED, OUTPUT);

  servo_left.attach(leftServo);
  servo_right.attach(rightServo);
  
  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //Print a message to the LCD.
  lcd.print("Hello, world!");

  //debug only
  Serial.begin(9600);
  Serial.print("Setup Complete\n");
}

void loop()
{
  lcd.clear();
  int duration;                  
  int tempo;
  int octave;
  int octave_pot;
  int tempo_pot;
  int i_note_index = 0;
  bool servo_dir = false; //to keep track of the servos' direction
  servo_left.write(90);
  servo_right.write(0);
  
    //power off
  while(digitalRead(powerSwitch)==0) 
  { 
    lcd.setCursor(0, 0);
    lcd.print("Off");
    digitalWrite(powerLED, LOW);
  }
  
  lcd.clear();
  
  //power on
  while (digitalRead(powerSwitch))
  { 
      lcd.setCursor(0, 0);
      lcd.print("On");
      digitalWrite(powerLED, HIGH);
      
      if(digitalRead(modeSwitch)){ //manual mode
        lcd.setCursor(0, 1);
        lcd.print("Manual");
        
        //read the tempo pot
        tempo_pot = analogRead(tempoPIN);
        tempo = song_tempo*float(tempo_pot)/TempoCal; //read the tempo POT       
        if(tempo<100){
          tempo = 100;
        }
      }
      else if (digitalRead(modeSwitch)==0){ //auto mode
        lcd.setCursor(0, 1);
        lcd.print("Auto  ");
        
        //get tempo and octave from the external song
        tempo  = 200;
        
        //sync the song

      }
      octave_pot = analogRead(octavePIN);
      if(octave_pot > 511) {
        octave = 5;
      } else {
        octave = 4;
      }
 
      lcd.setCursor(7, 0);
      lcd.print("Tempo: ");
      lcd.setCursor(13, 0);
      lcd.print(tempo);
      lcd.setCursor(7, 1);
      lcd.print("Octave: ");
      lcd.setCursor(14, 1);
      lcd.print(octave);
      //set the servos 
        if(servo_dir){
          servo_left.write(90-servo_counter);
          servo_right.write(servo_counter);
        }
        else{
          servo_left.write(servo_counter);
          servo_right.write(90-servo_counter);
        }
     
        servo_counter+=servoRate; 
        if(servo_counter>90){
          servo_counter = 0;
          servo_dir = !servo_dir; //flip direction
        }
      
      
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
  }
 }
