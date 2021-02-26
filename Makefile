# SHELL := /bin/bash

SRC = src
RTL = rtl
OBJ = obj

SOURCE = $(wildcard $(SRC)/*.c)  
ifneq ($(OS),Windows_NT)
	TARGETS = $(patsubst %.c, %, $(addprefix $(OBJ)/,$(notdir $(SOURCE))))
else
	TARGETS = $(patsubst %.c, %.exe, $(addprefix $(OBJ)/,$(notdir $(SOURCE))))
endif

CC = gcc  
CFLAGS = -Wall -g  

sw_sim: $(TARGETS)  
	./obj/crc_test.exe
	
ifneq ($(OS),Windows_NT)
$(OBJ)/%: $(SRC)/%.c
	$(CC) $< $(CFLAGS) -o $@  
else
$(OBJ)/%.exe: $(SRC)/%.c
	$(CC) $< $(CFLAGS) -o $@  
endif

hw_sim: rtl/tb_serial_crc.vvp
	vvp -N $<

rtl/tb_serial_crc.vvp: rtl/tb_serial_crc.v rtl/serial_crc.v rtl/parallel_crc.v
	iverilog -s testbench -o $@ $^ 

.PHONY:clean all  
clean:  
	rm -rf $(TARGETS)
	rm -rf rtl/*.vvp *.vcd