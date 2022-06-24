all: 
	gcc hw0401.c -lm -o hw0401
	gcc hw0402.c -lm -o hw0402
	gcc hw0403.c -lm -o hw0403
	gcc hw0404.c -lm -o hw0404

debug: 
	gcc hw0401.c -lm -o hw0401 -D DEBUG
	gcc hw0402.c -lm -o hw0402 -D DEBUG
	gcc hw0403.c -lm -o hw0403 -D DEBUG
	gcc hw0404.c -lm -o hw0404 -D DEBUG

clean: 
	rm -f hw0401
	rm -f hw0402
	rm -f hw0403
	rm -f hw0404

zip: 
	zip -r hw4 *.c *.h *.pdf Makefile lib
