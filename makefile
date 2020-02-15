all: crc_ccitt crc_test
	@echo Done

crc_test: crc_test.c
	gcc crc_test.c -o crc_test.exe

crc_ccitt: crc_ccitt.c
	gcc crc_ccitt.c -o crc_ccitt.exe
