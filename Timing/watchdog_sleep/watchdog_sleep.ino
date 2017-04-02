#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

volatile int f_wdt=1;
ISR(WDT_vect)
{
  if(f_wdt == 0)f_wdt=1;
}
void enterSleep(void)  
 {  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  sleep_enable(); 
  sleep_mode();  
  /** The program will continue from here. **/  
  /* First thing to do is disable sleep. */  
  sleep_disable(); 
  power_all_enable(); 
 }  

 void blinkPeriod(int milliSeconds, int mode){
  
      if(!mode)delay(milliSeconds);
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(milliSeconds);              // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      if(mode)delay(milliSeconds);  
 }
void setup() {
    Serial.begin(9600);
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  MCUSR &= ~(1<<WDRF); //reset flag of wdt   
  WDTCSR |= (1<<WDCE) | (1<<WDE); //In order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles).
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */      //set new watchdog timeout prescaler value
  WDTCSR |= _BV(WDIE);//enable wd interrupt
  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
}

void loop() {   
  Serial.println("_______________________Loop started_______________________"); 
  for(int i=0;i<20;i++) blinkPeriod(100,0);
  enterSleep();
  for(int i=0;i<10;i++) blinkPeriod(100,1);
  digitalWrite(13,HIGH);
  delay(1000);
}
