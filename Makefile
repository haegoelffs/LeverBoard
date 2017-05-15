# Makefile fuer AVR Assembler sources
# (c) Matthias Meier, 24.2.2016

# PROJECT = $(shell basename $(shell pwd))
PROJECT = myDrive
#PROJECT = system_tester_snippet
#PROJECT = drive_tester_snippet

OBJS = $(PROJECT).o System/ringbuffer.o System/logger.o System/loggerISR.o System/timer.o System/uart.o System/pwm.o System/adc.o System/ext_comperator.o System/systime.o startup.o run2.o
#OBJS = $(PROJECT).o System/ringbuffer.o System/logger.o System/timer.o System/uart.o System/pwm.o System/ext_comperator.o System/systime.o startup.o measureZeroCrossing.o
#OBJS = $(PROJECT).o System/ringbuffer.o System/logger.o System/timer.o System/uart.o System/pwm.o System/ext_comperator.o System/systime.o System/adc.o System/loggerISR.o

CC = avr-gcc

MCU = atmega2560
ADUDE_IF = /dev/ttyACM0
ADUDE_BAUD = 115200
ADUDE_PROT = wiring

CFLAGS = -mmcu=$(MCU) -Wall -O2 -DF_CPU=16000000UL -gdwarf-2

all: $(PROJECT).elf flash

# normal linker rule for C projectmperator.o System/systime.o startup.o run2.o
#OBJS = $(PROJECT).o System/ringbuffer.o System/logger.o System/timer.o System/uart.o System/pwm.o System/ext_comperator.o System/systime.o startup.o measureZeroCrossing.o
$(PROJECT).elf: $(OBJS)
	$(CC) $(CFLAGS) $+ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<  -o $@

%.hex: %.elf
	avr-objcopy -O ihex $<  $@

%.dis: %.elf
	avr-objdump  -D $<  > $@



flash: $(PROJECT).hex
	avrdude -p $(MCU) -c dragon_isp -P usb -e -U flash:w:$(PROJECT).hex:i
#	avrdude -F -c $(ADUDE_PROT) -p $(MCU) -P $(ADUDE_IF) -b $(ADUDE_BAUD) -U flash:w:$(PROJECT).hex:i


clean:
	rm -f $(PROJECT).dis $(PROJECT).elf $(PROJECT).hex

.PHONY: clean flash

