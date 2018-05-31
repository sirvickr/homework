/*
5.4. Создайте программу, в которой посимвольно с использованием фунции 
fgetc() читается указанный в командной строке файл, прочитанные символы 
выводятся в поток stdout, разделенные пробелами (например, для содержимого 
файла "Hello, world!" должно получиться "H e l l o , w o r l d ! "). Убедитесь, 
что, если файл содержит кириллицу в UTF-8, то результат получается некорректный.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("usage: %s file\n", argv[0]);
		return 0;
	}

	// открываем исходный файл на чтение
	FILE* file = fopen(argv[1], "r");
	if(!file) {
		fprintf(stderr, "failed to open source file\n");
		exit(EXIT_FAILURE);
	}

	while(!feof(file)) {
		int c = fgetc(file);
		if(EOF == c)
			break;
		printf("%c ", c);
	}
	printf("\n");
	
	// освобождаем ресурсы
	if(file)
		fclose(file);

	return 0;
}
