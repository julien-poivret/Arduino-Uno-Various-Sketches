/*
       Config the 10 bits ADC (raw study.)
       -> the purpose is to understand the peripheral of the chip 
       in order to build a "driver" for the peripheral. 
       ( in a practical use, an AnalogRead may do the trick, most of the time...)
       -> but better to understand hardware in order to track it's technological 
       evolution and features advantage, especially when it will be time to work one 
       a more advanced chip technologies. 

    
       Convert an analog signal in raw and succesives digital values.
    
^   sin x
     |
    ,|, 1    _....._
     |    ,="       "=.
     |  ,"             ".                           ,"
     |,"        ,        ".,          ,          ,,"
"""".*""""""""""|""""""""""|."""""""""|""""""""".|"""">
  ." |                       ".               ." __    x
,"   |                         "._         _,"   ||
    ,|,                  sm       "-.....-"
     |  -1
     |
 
        !!! Max Voltage 5 volts on ANALOG IN pin !!!
                                   
      When we measure an electrical value we must keep 
      in mind that noise is coming from many Electro Magnetic 
      source around the chip. SO, a 10uh in series with 100nf must 
      filter avcc (pin 20) from any parasitic noise around. 
            
            vcc----^^10UH^^--AVCC--|100NF|------GND
                              |
                              |
                      (chip DIP28 PIN20)
                          
      (The resonance attenuates parasitic high frequency pulses) 
      
      electrons are just mass induced by negatives charges 
      mass is from gravity hearth... so many sources of emi induce at the 
      same place and act as a perfect blender on the circuits... 
      so noise filtration/selectiom its a fundamental electronic topic.
      
      
      Buffering the output with a ring/circular buffer may prevent noises
      and allow a fast and more accurate signal reading at the price of a 
      dalay for filling the buffer before a noise free reading possibility.
      
      fast proccessing quote:
              *** before the instant the second is you ***
*/

// Arduino Uno Analog Digital Converter registers deffinition.

volatile uint8_t* _ADMUX = (volatile uint8_t*) 0x7C;   // Multiplexer
volatile uint8_t* _ADCSRA = (volatile uint8_t*) 0x7A;  // Status register A


volatile uint8_t* _ADCH = (volatile uint8_t*) 0x79;    // output High
volatile uint8_t* _ADCL = (volatile uint8_t*) 0x78;    // output Low

volatile uint8_t* _ADCSRB = (volatile uint8_t*) 0x7B;  // Status register B
volatile uint8_t* _DIDR0 = (volatile uint8_t*) 0x7E;   // Digital Input Disabled Register 0

volatile uint8_t* _PRR = (volatile uint8_t*) 0x64;   // Power reduction resistor.



void setup() {
  // put your setup code here, to run once:
  DDRB |= 0x21; // led 8 and 13 in OUTPUT mode.
  
  Serial.begin(9600);
  Serial.println("Hello");
  Serial.println(*_ADMUX,BIN);
  *_ADMUX &= ~0xC0; // reset bit 7 & 6.
  *_ADMUX |= 0x43;  // set ref internal 5v and Analog convertion on A3.
  Serial.println(*_ADMUX,BIN);

}

void loop() {
  // put your main code here, to run repeatedly
  PORTB ^= 0x1;
  delay(100);
  Serial.println(_ADC_read(),DEC);
}

uint16_t _ADC_read(void){
  *_PRR &= ~0x1; // disable power reduction to perform a read.
  *_ADCSRA |= 0x7; // Set the sampling rate at 16MHZ/128 sample /sec (125khz)
  *_ADCSRA |= 0xC0; // start 25 ADC conversion cycles. 
  PORTB |= 0x20; // led 13 On reading pending.
  while(!(*_ADCSRA & 0x10)){
    asm(""); // wait for ADIF flag (end of the 25 ADC Conversions)
  }
  PORTB &=~0x20; // led 13 off reading done.
  uint16_t adc_value = 0;
   adc_value |= * _ADCL;   // read low register first.
   adc_value |= *_ADCH<<8; // next copy high register.  
  *_ADCSRA &=~ 0x10; // clear conversion flag ADIF
  return adc_value;
}
