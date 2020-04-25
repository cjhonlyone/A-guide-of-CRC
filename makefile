SRC = src
RTL = rtl

all: $(SRC)/%.exe
	@echo Done

$(SRC)/%.exe: $(SRC)/%.c
	gcc -c $< -o $@ 

clean:
	rm -rf src/*.exe