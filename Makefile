
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
LDFLAGS=-lm

# define targets
TARGETS=Advanced-Matrix-Calculator

build: $(TARGETS)

Advanced-Matrix-Calculator: Advanced-Matrix-Calculator.c
	$(CC) $(CFLAGS) functii_2.c functii.c rotatie_matrice.c strassen.c list.c Advanced-Matrix-Calculator.c -o Advanced-Matrix-Calculator $(LDFLAGS)

pack:
	zip -FSr Proiect_Pclp3.zip Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
