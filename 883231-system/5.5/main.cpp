/*
5.5. Модифицируйте программу из предыдущего примера с использованием функции fgetwc(). 
Убедитесь, что программа корректно работает, если файл содержит символы в UTF-8.
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

int main(int argc, char* argv[])
{
	// установка локализации (чтобы работал printf("%c"))
	char* loc = setlocale(LC_ALL, "");
	if(loc == NULL) {
		fprintf(stderr, "failed to set locale\n");
		exit(EXIT_FAILURE);
	}
	//fprintf(stderr, "locale is set to %s\n", loc);

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
		wint_t c = fgetwc(file);
		if(WEOF == c)
			break;
		printf("%lc ", c);
	}
	printf("\n");
	
	// освобождаем ресурсы
	if(file)
		fclose(file);

	return 0;
}
