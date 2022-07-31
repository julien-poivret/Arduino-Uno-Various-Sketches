/*
                        (  Work only on 8 bits in it's form.  )
		   
Bp function description: 
                       
		      Define a sofware breakpoint and take a snap shot of a given register
                      at the breakpoint time then a push button is presssed for switching 
		      to an other one or simply continue the runtime.
		       
	       label: 
                      name of the breakpoint in char* exemple: "someting"
   
   breakpoint_number: 

                      the order of the break point in the runtime in (uin8_t).
                      -> a value of 255, make the breakpoint "out" of the 
                      chronological runtime an can be triggered at any time
                      without perturbing the runtime of other breakpoints 
                      usefull for Interrupt System Routine.
							   
                 REG: 
                      the 8 bit register value to watch (uint8_t)
                      the value is printed in (MSB first) binary format
                      with 4 bits space for easy reading.                  
     
*/

//////////////////////////////////////////////////                        just copy&paste from there
uint8_t debug_flag = 0;

void bp(char* label,uint8_t breakpoint_number, uint8_t REG){
	/* !!! Define this three next lines in setup()
	Serial.begin(9600); //  Serial port
	DDRC &= ~0x2;       //  PC1 as inpput 
	PORTC |= 0x2;       //  PC1 INPUT_PULLUP
	*/
	while(((PINC&0x2)>>1) && ((debug_flag==breakpoint_number) || (breakpoint_number==0xFF))){
		Serial.print( label);
		for(int8_t i=7;i>=0;i--){
			if(!i){
				Serial.println((REG&(1<<i))>>i,DEC);
			}else{
				if(i==3){
					Serial.print(" ");
				}
				Serial.print((REG&(1<<i))>>i,DEC);
			}
		}
		delay(250);
	}
  if(breakpoint_number!=0xFF){
    debug_flag++;
  }
}
//////////////////////////////////////////////////////////////////                      to there !!!

void setup(){
Serial.begin(9600);
DDRC &= ~0x2;                 // PC1 as inpput 
PORTC |= 0x2;                 // PC1 INPUT_PULLUP
DDRB |= 0x20;                 // the ON_BOARD led as OUTPUT. 
}

void loop(){
  PORTB ^= 0x20;
  delay(250);
  bp("brak point A",0,PINB);
  PORTB ^= 0x20;
  delay(250);
  bp("brak point B",1,PINB);
 }
