#!/bin/bash

# Replace with were Arduino is installed in your system.
AVR_DIR=/usr/share/arduino/hardware/tools/avr
SERIAL_PORT=/dev/ttyUSB0

SRC_MAIN=../src/main
SRC_MISC=$SRC_MAIN/misc
SRC_MPU=$SRC_MAIN/mpu6050
SRC_RAMPS=../src/ramps
BUILD=../build

avr-g++ -O -DF_CPU=16000000UL -DRAMPS -mmcu=atmega328p -c -o $BUILD/dobot.o $SRC_MAIN/dobot.cpp \
&& avr-g++ -O -DF_CPU=16000000UL -DRAMPS -mmcu=atmega328p -c -o $BUILD/calibrator.o $SRC_MISC/calibrator.cpp \
&& avr-g++ -O -DF_CPU=16000000UL -DRAMPS -mmcu=atmega328p -c -o $BUILD/queue.o $SRC_MISC/queue.cpp \
&& avr-g++ -O -DF_CPU=16000000UL -DRAMPS -mmcu=atmega328p -c -o $BUILD/ramps.o $SRC_RAMPS/ramps.cpp \
&& avr-g++ -O -DF_CPU=16000000UL -DRAMPS -mmcu=atmega328p -c -o $BUILD/mpu6050.o $SRC_MPU/mpu6050.c \
&& avr-g++ -O -DF_CPU=16000000UL -DRAMPS -mmcu=atmega328p -c -o $BUILD/twimastertimeout.o $SRC_MPU/twimastertimeout.c \
&& avr-g++ -mmcu=atmega328p \
$BUILD/dobot.o \
$BUILD/mpu6050.o \
$BUILD/twimastertimeout.o \
$BUILD/ramps.o \
$BUILD/calibrator.o \
$BUILD/queue.o \
-o $BUILD/dobot \
&& avr-objcopy -O ihex -R .eeprom $BUILD/dobot $BUILD/dobot-ramps.hex \
&& avrdude -patmega328p -carduino -P${SERIAL_PORT} -b57600 -D -Uflash:w:$BUILD/dobot-ramps.hex:i
