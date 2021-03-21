CC = gcc -Wall

all: clean runStatic runShared runDynamic 

static:
	make clean
	$(CC) -c bloclib.c 
	ar crs libbloclib.a bloclib.o
	$(CC) main.c -o main -L. -lbloclib 
	
shared:
	make clean
	$(CC) -c -fPIC bloclib.c
	$(CC) -shared -fPIC -o libbloclib.so bloclib.o
	$(CC) main.c -o main -L. -lbloclib -Wl,-rpath=`pwd`
	
dynamic:
	make clean
	$(CC) -c -fPIC bloclib.c -o bloclib.o 
	$(CC) -Wl,-rpath=. -fPIC -shared -o libbloclib.so bloclib.o 
	$(CC) -fPIC -L. -o main mainDynamic.c -ldl -D DYNAMIC
	
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f *.txt*.txt
	rm -f main
	
runStatic: static run clean
runShared: shared run clean
runDynamic: dynamic run clean
run:
	@echo  -Small-
	./main 1 create_table 1 merge_files a.txt:b.txt add_block 0 remove_block 0
	@echo  -Medium-
	./main 1 create_table 1 merge_files c.txt:d.txt add_block 0 remove_block 0
	@echo -Large-
	./main 1 create_table 1 merge_files e.txt:f.txt add_block 0 remove_block 0
	
	@echo -Few_Pairs-
	./main 3 create_table 3 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt
	@echo -Moderate Pairs-
	./main 8 create_table 16 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt
	@echo -Many Pairs-
	./main 32 create_table 32 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt
	@echo -addAndDelete-
	./main 6 create_table 6 merge_files e.txt:f.txt e.txt:f.txt  f.txt:g.txt g.txt:e.txt k.txt:g.txt i.txt:f.txt add_block 0 add_block 1 remove_block 0 add_block 0 remove_block 1 add_block 1 add_block 3 add_block 4 add_block5 remove_block 4 remove_block 3
	make clean 

