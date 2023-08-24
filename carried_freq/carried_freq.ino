/** 
 *  A basic and static example sketch to obtaine high frequency modulated stimulation.
 *  
 *  @author: Bethel Osuagwu
 *  @emai: bethel.osuagwu@gmail.com
 *  @lisence: MIT.
 *  
 *  Disclaimer: Use this code at your own risk.
 */
int LED =13;
int state=0;

//
float freq1=30.0;//frequency in Hz
float freq2=10000.0;//frequency in Hz. This is to be much higher than freq1

float freq1_period_ratio;//i.e in one period of freq1, how many periods of freq2 are there.

//
unsigned int t=0;
double freq1_signal=0.0;
double freq2_signal=0.0;


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(LED,OUTPUT);

  //
  freq1_period_ratio=freq2/freq1;

  delay(2000);
  //Serial.println("Ready");
}

void loop() {

  // Low frequency
  if(t<=freq1_period_ratio/2){
    state=1;
  }else{
    state=0;
  }

  // Reset t when it reaches one period of freq1, which happens when we reach 1 period ratio
  if(t>=(freq1_period_ratio)){
    t=0;
  }
 

  // High frequency
  if(state==1){
     digitalWrite(LED,HIGH);
  }
  delayMicroseconds(70-20);

  //
  if(state==1){// turn OFF if it is ON 
    digitalWrite(LED,LOW);
  }
  delayMicroseconds(20);
  t=t+1;
}
