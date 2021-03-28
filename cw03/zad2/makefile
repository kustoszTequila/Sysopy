CC = gcc -Wall

all: clean static shared run 

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
	
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f *.txt*.txt
	rm -f main
	
run:
	@echo  -Small-
	./main 1 create_table 1 merge_files a.txt:b.txt add_block 0 remove_block 0
	@echo  -Medium-
	./main 1 create_table 1 merge_files c.txt:d.txt add_block 0 remove_block 0
	@echo -Large-
	./main 1 create_table 1 merge_files e.txt:f.txt add_block 0 remove_block 0
	
	@echo -Few_Pairs-
	./main 4 create_table 4 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt e.txt:f.txt
	@echo -Moderate Pairs-
	./main 16 create_table 16 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt e.txt:f.txt f.txt:d.txt f.txt:h.txt h.txt:i.txt i.txt:j.txt l.txt:k.txt k.txt:e.txt f.txt:j.txt
	@echo -Many Pairs-
	./main 64 create_table 64 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt a.txt:b.txt b.txt:c.txt c.txt:d.txt a.txt:c.txt a.txt:d.txt e.txt:f.txt f.txt:e.txt e.txt:a.txt
	@echo -addAndDelete-
	./main 10 create_table 10 merge_files e.txt:f.txt e.txt:f.txt  f.txt:g.txt g.txt:e.txt k.txt:g.txt i.txt:f.txt add_block 0 add_block 1 remove_block 0 add_block 0 remove_block 1 add_block 1 add_block 3 add_block 4 add_block 5 remove_block 4 remove_block 3 merge_files a.txt:b.txt b.txt:c.txt c.txt:d.txt e.txt:f.txt add_block 1 add_block 2 remove_block 1 remove_block 2
	make clean 

