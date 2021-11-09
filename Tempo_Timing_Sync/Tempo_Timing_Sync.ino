int sensorPin = A0, octave, numReadings, rise_i, fall_i, i;
double tempo, startPos;
const int edgesLength = 4;
bool high, start;
double value, threshold;
unsigned long timeFromStart;

int beats[54] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};


void setup() {
  Serial.begin(115200); // set the data rate for the Serial communication
  pinMode(A0, INPUT); // analog pin used to connect the sensor
  value = smoothInput(A0);
  octave = 5;
  if(octave == 2) {
    threshold = 0.2;
    numReadings = 3;
  } else if(octave == 3) {
    threshold = 0.3;
    numReadings = 3;
  } else if(octave == 4) {
    threshold = 0.25;
    numReadings = 3;
  } else if(octave == 5) {
    threshold = 0.28;
    numReadings = 6;
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
  int value = smoothInput(sensorPin);
  Serial.println((double)analogRead(sensorPin)/512);
//  start = false;
//  //Serial.println("start loop");
//  while(!start) {
//    findTempo(tempo);
//    Serial.println(tempo);
//    start = true;
//  }
//  while(start) {
//    //play song
//  }
//  //value = smoothInput(A0);
//  //Serial.println(value);
  
}

int smoothInput(int sensorPin) {
  double value = 0, data = 0;

  for (int i = 0; i < numReadings; i++){
    //Serial.print("data: ");
    data = (double)analogRead(sensorPin)/512-1;
    data = abs(data);
//    Serial.println(data);
    value += data;
    //Serial.println(value);
  }

  // Take an average of all the readings.
  value = value / (double)numReadings;

  if(value > threshold)
    return 1;
  else
    return 0;
}

void findTempo(double &tempo) {
  unsigned long rise[edgesLength], fall[edgesLength];
  //Serial.println("find tempo");
  int i = 0, j = 0;
  while(i < edgesLength && j < edgesLength) {
      value = smoothInput(A0);
      //Serial.println(value);
      if(!high && value == 1) { //rising edge
          rise[i] = millis();
          i++;
      } else if(high && value == 0) { //falling edge
          fall[j] = millis();
          j++;
      } 
  }
//  Serial.println(rise[1]-fall[0]);
//  Serial.println(rise[2]-fall[1]);
//  Serial.println(rise[3]-fall[2]);

  double note = ((rise[1] - fall[0]) + (rise[2] - fall[1]) + (rise[3] - fall[2])) / 3;
//  Serial.println(note);
  tempo = note / 1000; //beats per second
}
//
//double findStart(int falling_edges[], int rising_edges[]) {
//  int diff = 0;
//
//  for(int j = 0; j < (edgesLength-1); j++) {
//    diff += falling_edges[j+1] - rising_edges[j];
//  }
//  
//  return (diff / (edgesLength - 1));
//}
