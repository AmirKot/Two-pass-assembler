my_assem:assembler.o  assem.o first_pass.o hash.o errors.o second_pass.o
	gcc  -Wall -g -pedantic assembler.o assem.o errors.o first_pass.o second_pass.o hash.o -o my_assem
assembler.o:assembler.c assembler.h first_pass.h
	gcc  -Wall -g -pedantic -c assembler.c -o assembler.o
assem.o: assem.c assembler.h
	gcc  -Wall -g -pedantic -c assem.c -o assem.o 
first_pass.o:first_pass.c first_pass.h
	gcc  -Wall -g -pedantic -c first_pass.c -o first_pass.o
second_pass.o:second_pass.c second_pass.h
	gcc  -Wall -g -pedantic -c second_pass.c -o second_pass.o
hash.o:hash.c hash.h 
	gcc  -Wall -g -pedantic -c hash.c -o hash.o
errors.o:errors.c libary.h 
	gcc  -Wall -g -pedantic -c errors.c -o errors.o
clean:
	rm -f my_assem assembler.o assem.o first_pass.o second_pass.o hash.o errors.o