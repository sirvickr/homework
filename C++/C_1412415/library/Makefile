all: books

books: main.o book.o bilist.o
	gcc main.o book.o bilist.o -o books

main.o: main.c bilist.h book.h
	gcc -c main.c

bilist.o: bilist.c bilist.h
	gcc -c bilist.c

book.o: book.c book.h
	gcc -c book.c

clean:
	rm -fr *.o books
