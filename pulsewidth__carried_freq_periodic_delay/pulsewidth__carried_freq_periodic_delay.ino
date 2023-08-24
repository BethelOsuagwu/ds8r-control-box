/* 
 *  A sketch to obtain high frequency modulated stimulation. This includes a 
 *  configurable pulsewidth for th e low frequency.
 *  The sketch modulates a given low frequency with a given high frequency.
 *  The resulting signal is sent to a set output port.
 *  
 *  @author: Bethel Osuagwu
 *  @emai: bethel.osuagwu@gmail.com
 *  @lisence: MIT.
 *  @updated: 11/02/2023
 *  
 *  Disclaimer: Use this code at your own risk.
 */


//===================================================================
// CHANGE THESE.
//===================================================================
float LOW_FREQ=30.0;//frequency in Hz
float LOW_FREQ_PULSEWIDTH=1;// The pulsewidth of the low frequency in miliseconds. Must not be greater than 1/LOW_FREQ, otherwise the program won't work.
float HIGH_FREQ=10000.0;//frequency in Hz. This is to be much higher than LOW_FREQ. 
float DUTY=0.8;// Duty cycle as fraction. The fraction of time the high frequency is UP within its period. Note: if using the DS8R then this is just the width of the trigger as the actual stimulus pulsewidth is determined by the setting on the stimulator.
int OUTPUT_PORT =12;// output pin

//===================================================================
// NO NEED TO CHANGE BELOW.
//===================================================================

bool DEBUG=true;// In debug mode when true.
bool LOW_FREQ_OFF=false;//Turn off the low frequency when true.
/*
 * DEBUG NOTE: HIGH_FREQ@10k=> if output Fs@50k, then 100 samples => 20PERIODs.
 */


// - The period should not be greater than the exact and max period: (1.0/HIGH_FREQ)*1000000. 
// - If the system is slow/has heavy overhead then the correct period may be less than the max. 
// - You can experiment with periods lower than the max period until HIGH_FREQ is correctly produced. 
unsigned short SAMPLE_TIME=(1.0/HIGH_FREQ)*1000000;// System SAMPLE_TIME in microseconds.


// The cycles of LOW_FREQ relative to the period. i.e in one period of LOW_FREQ, how many system periods are there.
float LOW_FREQ_CYCLES=((1.0/SAMPLE_TIME)/LOW_FREQ) *1000000;// or simply =HIGH_FREQ/LOW_FREQ;
float LOW_FREQ_DUTY=((LOW_FREQ_PULSEWIDTH/1000) * LOW_FREQ);// The duty cycle of the low frequency in fraction. It must not be greater than 1.
float LOW_FREQ_PULSEWIDTH_CYCLES=LOW_FREQ_DUTY * LOW_FREQ_CYCLES;// This expression gives the number of samples for a given pulsewidth.

//
unsigned short DUTY_UP;// The time in miscroseconds the high frequency stays in the UP state. The value must be adequate for delayMicroseconds(). 
unsigned short DUTY_DOWN;// The time in microseconds the high frequency signal stays in the DOWN state.  The value must be adequate for delayMicroseconds().

//
unsigned int t=0;// System step counter for the current number of cycles of the high frequency within one cycle of the low frequency. 

// Store overhead timer start
unsigned long tic;

bool state=false;// the state of the output pin. false=>LOW, true=>HIGH.

bool HAS_ERROR=false;// True when there is error.

void setup() {
  // put your setup code here, to run once:

  // Pre-validation
  if(LOW_FREQ_DUTY>1){
    HAS_ERROR=true;
  }
  if(HIGH_FREQ < LOW_FREQ){
    HAS_ERROR=true;
  }
  if(DUTY>1){
    HAS_ERROR=true;
  }

  //
  if(DEBUG || HAS_ERROR){
    Serial.begin(9600);
    Serial.println("initialising");
  }
  pinMode(OUTPUT_PORT,OUTPUT);
  delay(2000);// Arbitrary time to let the system ready itself.


  //
  DUTY_DOWN=0;
  DUTY_UP=0;
  tic=micros();
}

void loop() {

  //============================================
  // Validation:
  // Check parameters and halt if all is not fine.
  //============================================
  if(HAS_ERROR){
    Serial.println("Error: Check parameters");
    return;
  }

  
  // ===========================================
  // Low frequency:
  // Determine if the low frequency is UP or DOWN.
  // ============================================
  if(LOW_FREQ_OFF){
    state=true;
  }else{
    if(t<=LOW_FREQ_PULSEWIDTH_CYCLES){
      state=true;
    }else{
      state=false;
    }
  }
  
  // Reset t when it reaches one period of LOW_FREQ
  if(t>=(LOW_FREQ_CYCLES)){
    t=0;
  }
  

  // =======================================
  // High frequency:
  // Now pulse the output to create the high frequency.
  //================
  if(state){// The high frequency is only UP if the low frequency is UP.
     digitalWrite(OUTPUT_PORT,HIGH);
  }

  // Measure all overhead
  unsigned long toc =micros()-tic;//elapsed time
  
  // Start time to measure all overhead. NOTE that we are starting the 
  // time here so that we can also capture the time spent on the 
  // computations below.
  tic=micros();

  // Correct the sample time with effective overhead, taking previous
  // explicit delays into account.
  int SAMPLE_TIME_CORRECTED =SAMPLE_TIME-(toc-DUTY_UP-DUTY_DOWN);
  DUTY_UP= (DUTY)*SAMPLE_TIME_CORRECTED ;
  
  delayMicroseconds(DUTY_UP);

  //
  if(state){// turn OFF if it is ON 
    digitalWrite(OUTPUT_PORT,LOW);
  }

  DUTY_DOWN=SAMPLE_TIME_CORRECTED-DUTY_UP;
  if(0<(DUTY_DOWN)){// Pedantic check; there should be no need for this
    delayMicroseconds(DUTY_DOWN);
  }else{
    DUTY_DOWN=0;
  }

  // Do other work
  sub_system(t);
  
  t=t+1;
}

/**
 * Do work in a subsystem.
 * @param t Current number of system periods within the period for LOW_FREQ.
 */
void sub_system(unsigned int t){
  // Do work here
}
