PROGRAMS = finds

.PHONY: all clean

all: $(PROGRAMS)

%: %.c
	gcc -Wall -g $^ -o $@

clean: 
	rm -rf *~ $(PROGRAMS) 