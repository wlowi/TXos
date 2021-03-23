###############################################################################
# Makefile for the project TXos
###############################################################################

## General Flags
PROJECT = TXos
MCU = atmega328p
TARGET = TXos.elf
CC = avr-gcc
CPP = avr-g++
INCLUDES = -I..

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=c++11 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,--gc-sections,-Map=$(PROJECT).map

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = TXos.o Dispatcher.o PPM.o AnalogInput.o DigitalInput.o Buffer.o Serial.o ProcessChain.o \
ServoReverse.o ServoSubTrim.o ServoLimit.o ModeSelector.o ModelSelector.o \
ConfigBlock.o Configurable.o SystemConfig.o ModelConfig.o

## Objects explicitly added by the user
#LINKONLYOBJECTS = 

#.SUFFIXES:
#.SUFFIXES: .o .c

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size

## Compile
TXos.o: ../TXos.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Dispatcher.o: ../Dispatcher.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PPM.o: ../PPM.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

AnalogInput.o: ../AnalogInput.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DigitalInput.o: ../DigitalInput.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Buffer.o: ../Buffer.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Serial.o: ../Serial.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ProcessChain.o: ../ProcessChain.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ServoReverse.o: ../ServoReverse.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ServoSubTrim.o: ../ServoSubTrim.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ServoLimit.o: ../ServoLimit.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ModeSelector.o: ../ModeSelector.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ModelSelector.o: ../ModelSelector.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ConfigBlock.o: ../ConfigBlock.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Configurable.o: ../Configurable.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SystemConfig.o: ../SystemConfig.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ModelConfig.o: ../ModelConfig.cpp ../*.h
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size ${TARGET}

## AVR

check:
	avr check

write:
	avr write flash $(PROJECT).hex

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(TARGET) dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

