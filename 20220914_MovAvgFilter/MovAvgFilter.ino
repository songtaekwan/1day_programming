#define TRIG 9 
#define ECHO 8 
#define MASK_LENGTH 10

int raw_array[MASK_LENGTH] = {0,};
int raw_array_index = 0;

void setup(){
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println("Distance,Avg");
}

float MovAvgFilter()
{
  int i = 0;
  int sum = 0;
  float pre_avg = 0;
  float avg = 0;
  
  for (i = 0; i < MASK_LENGTH ; i++) 
  {
    sum += raw_array[i];
    //printf("sum: %d\n",sum);
  }
  
  pre_avg = (float)sum / (MASK_LENGTH);
  avg = pre_avg + (raw_array[raw_array_index]/MASK_LENGTH) - (raw_array[0]/MASK_LENGTH);  
  pre_avg = avg;
  
  return avg;
}

void insertIntoRawArray(int value)
{
    raw_array[raw_array_index] = value;
    raw_array_index++;

    if (raw_array_index >= MASK_LENGTH) 
    {
      raw_array_index = 0;
    }
}

void loop(){
  long duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn (ECHO, HIGH); 
  distance = duration * 17 / 1000; 
  
  Serial.print(distance); //거리값(cm)
  Serial.print(","); 
  
  insertIntoRawArray(distance);
  
  Serial.println(MovAvgFilter()); 
  delay(200);
}
