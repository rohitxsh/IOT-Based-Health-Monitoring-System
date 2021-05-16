int UpperThreshold = 518;
int LowerThreshold = 490;
    int reading = 0;
    float BPM = 0.0;
    bool IgnoreReading = false;
    bool FirstPulseDetected = false;
    unsigned long FirstPulseTime = 0;
    unsigned long SecondPulseTime = 0;
    unsigned long PulseInterval = 0;

    void setup(){
      Serial.begin(9600);
    }

    void loop(){

      reading = analogRead(0);
      if(reading > UpperThreshold && IgnoreReading == false){
        if(FirstPulseDetected == false){
          FirstPulseTime = millis();
          FirstPulseDetected = true;
        }
        else{
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          FirstPulseTime = SecondPulseTime;
        }
        IgnoreReading = true;
      }
      if(reading < LowerThreshold){
        IgnoreReading = false;
      }

      BPM = (1.0/PulseInterval) * 60.0 * 1000;
       // uncomment these lines in case you want to view the various values in the console.....
      /*Serial.print(reading);
      Serial.print("\t");
      Serial.print(PulseInterval);
      Serial.print("\t");*/
      Serial.print(BPM);
      Serial.println(" BPM");
      Serial.flush();
    }
