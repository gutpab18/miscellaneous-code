.PHONY: all clean

all: my_echo

my_echo: my_echo.o
	gcc my_echo.o -o my_echo -g

my_echo.o: my_echo.c
	gcc my_echo.c -c -o my_echo.o -g

clean:
	rm -f my_echo.o my_echo