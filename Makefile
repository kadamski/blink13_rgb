F_CPU = 1200000
MCU = attiny13a
NAME = blink
OBJS = blink.c

#################
DUDEPORT = /dev/ttyACM0
DUDEPROGRAMER = usbasp
DUDEMCU = t13

#################

CC = avr-gcc
CFLAGS = -Os -DF_CPU=$(F_CPU)UL -mmcu=$(MCU) -Wall -std=gnu99

#################
all: $(NAME) size

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(NAME).hex: $(NAME)
	avr-objcopy -O ihex -R .eeprom $< $@

flash: $(NAME).hex
	avrdude -F -V -c $(DUDEPROGRAMER) -p $(DUDEMCU) -P $(DUDEPORT) -U flash:w:$<

readfuse: $(NAME).hex
	avrdude -F -V -c $(DUDEPROGRAMER) -p $(DUDEMCU) -P $(DUDEPORT) -U lfuse:r:-:h -U hfuse:r:-:h

size: $(NAME).hex $(NAME).lst
	avr-size $<

$(NAME).lst: $(NAME)
	avr-objdump -h -S $< > $@

clean:
	rm -rf $(NAME) $(NAME).hex
