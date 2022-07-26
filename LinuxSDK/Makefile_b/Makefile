"Makefile_vb"

"Experimental Makefile for toolchain study ------------------------------------------------------------------"
" For a practical use arduino IDE is just the right match, that's not the point.
" but for a learning point of view time come when you will need to see behind the preprocessing .ino
" file and see how you can boot the chip in pur c language with just a ship manufacturer startup file
" it's ONLY from there that you will be able to add layers of complexcity without loosing your mind.
" and hold a "kind of knowledge" making you ready to deal with other programers library/linking/addaptation
" and maintining the link for next generations.
"
" That why this.
" 
"So far it's nothing of unknow.



FILENAME = blink
PORT = /dev/ACM0.....
DEVICE = atmega328p
PROGRAMMER = arduino
BAUD = 115200
COMPILE = avr-gcc -WAll -Os -mmcu=$(DEVICE)

default:compile upload clean

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf
upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(PORT) -U flash:w:$(FILENAME).hex:i

clean:
	rm $(FILENAME).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex
