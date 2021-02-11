##

SRC = $(wildcard src/*.c)
OBJ = $(addprefix build/,$(notdir $(SRC:.c=.rel)))
vpath %.c src
OPT = -mmcs51 -Iinc

all: build/output.hex

build/output.hex: build/output.ihx
	packihx $< > $@

build/output.ihx: $(OBJ) | build
	sdcc $(OPT) -o $@ $^

build/%.rel: %.c | build
	sdcc $(OPT) -c -o $@ $<

build:
	mkdir build