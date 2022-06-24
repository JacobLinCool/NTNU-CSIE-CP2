all: prepare
	gcc hw0201.c -lm -o hw0201
	gcc hw0202.c -lm -o hw0202
	gcc hw0203.c madoka.o -lm -o hw0203
	gcc hw0204.c fraction.o -lm -o hw0204
	gcc hw0205.c -lm -o hw0205

clean: 
	rm -f hw0201
	rm -f hw0202
	rm -f hw0203
	rm -f hw0204
	rm -f hw0205
	rm -f madoka.o
	rm -f fraction.o
	rm -f hw2.zip

prepare:
	gcc madoka.c -c -o madoka.o
	gcc fraction.c -c -o fraction.o

zip: 
	zip hw2 *.c *.h **/*.h **/**/*.h *.pdf Makefile
