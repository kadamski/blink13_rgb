F_CPU := 1200000
MCU := attiny13a
NAME := blink
OBJS := blink.c

DUDEMCU := t13

include ../avr-makefile/Makefile
