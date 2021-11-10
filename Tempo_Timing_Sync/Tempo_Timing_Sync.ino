int sensorPin = A2, octave, numReadings, rise_i, fall_i, i;
double tempo, startPos;
const int edgesLength = 7;
bool high, start;
double value, threshold;
unsigned long timeFromStart;

int beats[54] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};


void setup() {
  Serial.begin(2000000); // set the data rate for the Serial communication
  pinMode(A2, INPUT); // analog pin used to connect the sensor
  value = smoothInput(A2);
  octave = 5;
  if(octave == 2) {
    threshold = 0.2;
    numReadings = 3;
  } else if(octave == 3) {
    threshold = 0.3;
    numReadings = 3;
  } else if(octave == 4) {
    threshold = 7;
    numReadings = 19;
  } else if(octave == 5) {
    threshold = 4;
    numReadings = 10;
  } else if(octave == 6) {
    threshold = 0.2;
    numReadings = 3;
  } else if(octave == 7) {
    threshold = 0.2;
    numReadings = 3;
  }

//  start = false;
//  //Serial.println("start loop");
//  while(!start) {
//    findTempo(tempo);
//    Serial.println(tempo);
//    start = true;
//  }
}


void loop() {
//  Serial.println(value);
//  start = false;
  Serial.println("start loop");
  start = false;
  while(!start) {
    findTempo(tempo);
//    Serial.println(tempo);
//    start = true;
  }
//  while(start) {
//    //play song
//  }
//  //value = smoothInput(A2);
//  //Serial.println(value);
  
}

int smoothInput(int sensorPin) {
  double value = 0, data = 0;

  for (int i = 0; i < numReadings; i++){
    //Serial.print("data: ");
    data = analogRead(sensorPin)-151;
    data = abs(data);
//    Serial.println(data);
    value += data;
    //Serial.println(value);
  }

  // Take an average of all the readings.
  value = (double)value / (double)numReadings;
//  Serial.println(value);
  //Serial.println(" ");

  if(value > threshold)
    return 1;
  else
    return 0;
}

void findTempo(double &tempo) {
  unsigned long rise[edgesLength], fall[edgesLength];
  //Serial.println("find tempo");
  int i = 0, j = 0;
  double total;
  while(i < edgesLength || j < edgesLength) {
      value = smoothInput(A2);
      for(int i = 0; i < 5; i++) {
        value += smoothInput(sensorPin);
      }
      value = value/5;
      //Serial.println(value);
      if(!high && value == 1) { //rising edge
          high = true;
          rise[i] = millis();
          Serial.print("rise: ");
          Serial.println(rise[i]);
          i++;
      } else if(high && value == 0) { //falling edge
          high = false;
          fall[j] = millis();
//          Serial.print("fall: ");
//          Serial.println(fall[j]);
          j++;
      }
  }

  for(int i = 0; i < (edgesLength-1); i++) {
    Serial.print("difference: ");
    Serial.println(rise[i+1] - fall[i]);
    if((rise[i+1] - fall[i] >= 100) && (rise[i+1] - fall[i] < 1000))
      total += rise[i+1] - fall[i];
  }
//  Serial.println(rise[1]-fall[0]);
//  Serial.println(rise[2]-fall[1]);
//  Serial.println(rise[3]-fall[2]);

  double note = total / (edgesLength-1);
  Serial.println(note);
  tempo = note; //beats per second
  Serial.println(tempo);
}
