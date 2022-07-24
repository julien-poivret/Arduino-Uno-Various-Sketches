/* 
                                Technical explanation:

Info primary Block (of 4 bits):

      Dec        Bin          Hex
       0           0          0x0
       1           1          0x1  
       2          10          0x2
       3          11          0x3
       4         100          0x4
       5         101          0x5
       6         110          0x6
       7         111          0x7
       8        1000          0x8
       9        1001          0x9
      10        1010          0xA
      11        1011          0xB
      12        1100          0xC
      13        1101          0xD
      14        1110          0xE
      15        1111          0xF

  then you can conbine two blocks...
    8 bits value  -->   0x3D ---> (11+1101) ---> 111101 

    And so... with Hexadecimal you can handle 8 bits value in a more human "friendly way"....
    that why memory is often written in hexadecimal rather than binary 0x3D vs B111101
    specially on 32 or 64bits registers...
    4 bits  -> 0xF
    8 bits  -> 0xFF
    16 bits -> 0xFFFF
    32 bits -> 0xFFFFFFFF
    64 bits -> 0xFFFFFFFFFFFFFFFF
    no way to remind a radom 64bits number sequence in binary: 
    B1111111111011111101011111110101101111111111111111111111111111111
    crazy... but not for computers... and that's computing !
    
    By the way the human can create smarter machine than himself 
    but can machines create humans smarter than machine too ? 

---------------------------------------------------------------------------------------
                     
                     The 8 bits "on chip" REGISTER mystery.

exemple of 8 bits RAM memory on microcontroler or 8 bits computer:

physical adress       data        value
            0x0       00000000
            0x1       00000000
            0x2       00000000
            0x3       01110010 --> 0x72
            0x4       00000000
            0x5       00000000
            0x6       00000000


c/c++ Hardware physical register definition and manipulation:

-------------------------------------------------------------
code:
volatile uint8_t* pointer_register = (volatile uint8_t*) 0x3;
-------------------------------------------------------------

    if you extract the value at 0x3 (physical adress register on ram)  
    you get he value 0x72 -> ( 114 in decimal )"

-------------------------------------------------------------
code:
Serial.println(*pointer_register,DEC); 
-------------------------------------------------------------

        you get the value 114 printed on Serial
        from            *pointer_register 
   ( classic variable value from c/c++ pointer adress )

*/


/* 
   Start of the arduino code. 
   Setting the 8 bits register of the chip ATMEGA328P U (Arduino Uno)
   for interrupt on INT0 (PD2).
*/

//Output pin D physical adress definition
volatile uint8_t* _PIND = (volatile uint8_t*) 0x29;// pin status and config
volatile uint8_t* _DDRD = (volatile uint8_t*) 0x2A;// port direction
volatile uint8_t* _PORTD = (volatile uint8_t*) 0x2B;// pin set or reset

//Output pin B physical adress definition
volatile uint8_t* _PINB = (volatile uint8_t*) 0x23;// pin status and config
volatile uint8_t* _DDRB = (volatile uint8_t*) 0x24;// port direction
volatile uint8_t* _PORTB = (volatile uint8_t*) 0x25;// pin set or reset

//Interrupt REGISTER settings physical adress definition
volatile uint8_t* _SREG = (volatile uint8_t*) 0x5F;  // Global interupt
volatile uint8_t* _EICRA = (volatile uint8_t*) 0x69; // Raising or Falling edge or other
volatile uint8_t* _EIMSK = (volatile uint8_t*) 0x3D; // int0 or int1

volatile uint8_t flag = 0; // global variable C++ optimisation disabled (volatile).

void setup() {

  Serial.begin(9600);

  // Set input button and pull-up mode.
  *_DDRD &= ~0x4; // set PD2 as an input.
  *_PORTD |= 0x4; // pullup mode (20k in series internal before switch).

  *_DDRB |= 0x20; //set PB5 as an output.

  // Config interrupt.
  *_SREG |= 0x80; // enable globla interrupt always on by default (just for exemple).
  // Serial.println("SREG");
  // Serial.println(*_SREG, HEX);
  // delay(500);

  *_EICRA &= ~0xf;
  *_EICRA |= 0x3; // Interrupt in raising-edge mode.
  // Serial.println("EICRA");
  // Serial.println(*_EICRA, HEX);
  // delay(500);



  *_EIMSK |= 0x1; // int0 enabled on PD2.
  // Serial.println("EIMSK");
  // Serial.println(*_EIMSK, HEX);
  // delay(500);

}

void loop() {
  // Serial.println(*_PINB, HEX);

  delay(700);

}

// Poor debonce will be inprove not the subject of the study. 
ISR(INT0_vect) {
  if (flag < 5) {
    flag++;
  }
  if (flag >=3) {
    *_PORTB ^= 0x20;
    //Serial.println("Trigger");
    flag = 0;
  }
}
