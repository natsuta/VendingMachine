SOURCES=vm.c vm_menu.c vm_options.c vm_stock.c vm_coin.c utility.c
OBJECTS=vm.o vm_menu.o vm_options.o vm_stock.o vm_coin.o utility.o
HEADERS=vm.h vm_menu.h vm_options.h vm_stock.h vm_coin.h utility.h vm_system.h
PROGRAM=vm
DEBUG=-g
FLAGS=-ansi -pedantic -Wall $(DEBUG)

all: $(OBJECTS)
	gcc $(FLAGS) -o $(PROGRAM) $(OBJECTS)

clean:
	rm -f *.o $(PROGRAM) core

%.o: %.c $(HEADERS)
	gcc $(FLAGS) -c $< -o $@
	
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile Requirement13Part1.txt Requirement13Part2.txt

