#define N 100000000
#define min 4


const int analogPin = A0;
volatile int amplitude = 0;
int maxAmplitude = 0;
const unsigned long timer = 500UL;
float volt = 0; 
float volt_eds;
//float amlitudeAverege[N];
int count = 0;
float sum = 0;
void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  //attachInterrupt(digitalPinToInterrupt(analogPin), calculateAmplitude, CHANGE);
}

void loop() {
  maxAmplitude = analogRead(analogPin);
 if (maxAmplitude > min && count < N) {
    //maxAmplitude = analogRead(analogPin);
    //volt = (float)(maxAmplitude*5.0)/1024.0;
    volt_eds = 1.1;
    sum += (float)(maxAmplitude*volt_eds)/1024.0;
    //Serial.println(amlitudeAverege[count]);
    ++count;
  }else if(maxAmplitude < min){

  }
  else{
    Serial.println(sum/count,6);
    sum = 0.0;
    count = 0;
  }
  //delay(timer);
}

