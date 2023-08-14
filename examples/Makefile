## a really simple makefile shown in the readme

src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = #-lGL -lglut -lpng -lz -lm
CFLAGS = -Wall -pedantic -g

cydonia_example: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) cydonia_example