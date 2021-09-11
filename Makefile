MCU=atmega32u4
MCU2=m32u4
F_CPU=8000000UL
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-v -std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
PORT=/dev/ttyS10
TARGET=main
SRCS=main.c snake.c

all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -v -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex
	avr-size -C --mcu=${MCU} ${TARGET}.bin

flash:
	avrdude -p ${MCU} -c avr109 -U flash:w:${TARGET}.hex:i -F -P ${PORT}

clean:
	rm -f *.bin *.hex

