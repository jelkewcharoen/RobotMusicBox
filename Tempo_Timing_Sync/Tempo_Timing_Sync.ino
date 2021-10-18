int sensorPin = A0, octave, numReadings, rise_i, fall_i, i;
double tempo, startPos;
const int edgesLength = 5;
bool state, init_state;
double value, threshold;
int falling_edges[edgesLength];
int rising_edges[edgesLength];

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
    numReadings = 5;
  } else if(octave == 6) {
    threshold = 0.2;
    numReadings = 3;
  } else if(octave == 7) {
    threshold = 0.2;
    numReadings = 3;
  }

  if(value == 0) {
    init_state = false;
    state = false;
  } else {
    init_state = true;
    state = true;
  }

  rise_i = 0;
  fall_i = 0;
  i = 0;
}


void loop() {
  value = smoothInput(A0);
  //Serial.println(value);
  if(state && value == 0) { //falling edge
    //Serial.println("fall");
    falling_edges[fall_i] = i;
    fall_i++;
    state = false;
  }
  if(!state && value == 1) { //rising edge
    //Serial.println("rise");
    rising_edges[rise_i] = i;
    rise_i++;
    state = true;
  }
  if(rise_i == 4 || fall_i == 4) {
    Serial.print("falling: ");
    for(int k = 0; k < edgesLength; k++) {
      Serial.print(falling_edges[k]);
      Serial.print(", ");
    }
    Serial.println();
    Serial.print("rising: ");
    for(int k = 0; k < edgesLength; k++) {
      Serial.print(rising_edges[k]);
      Serial.print(", ");
    }
    Serial.println();
    tempo = findTempo(falling_edges, rising_edges);
    rise_i = 0;
    fall_i = 0;
    i = 0;
    Serial.println(tempo);
  }
  i++;
  //Serial.println(i);
}

int smoothInput(int sensorPin) {
  double value = 0, data = 0;

  for (int i = 0; i < numReadings; i++){
    //Serial.print("data: ");
    data = (double)analogRead(sensorPin)/512-1;
    data = abs(data);
    //Serial.println(data);
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

double findTempo(int falling_edges[], int rising_edges[]) {
  int diff = 0;
  for(int j = 0; j < (edgesLength-1); j++) {
    diff += rising_edges[j+1] - falling_edges[j];
  }
//  Serial.println(diff);
  return ((diff / (edgesLength - 1)));
}

double findStart(int falling_edges[], int rising_edges[]) {
  int diff = 0;

  for(int j = 0; j < (edgesLength-1); j++) {
    diff += falling_edges[j+1] - rising_edges[j];
  }
  
  return (diff / (edgesLength - 1));
}
