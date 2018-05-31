/*
4. Создайте собственный упрощенный аналог утилиты chmod:
chmod [OPTION]... MODE[,MODE]... FILE
Для упрощенного аналога режим указывать в формате, который используется в выводе ls -l:
-rw-r--r-- 1 root root 2484 May 29 11:43 /etc/passwd
Т.е. ./mychmod 0755 файл или ./mychmod -rwxr-xr-x файл.
варианты запуска и проверки:
./task -rw-rw-rw- file
ls -l file
./task -rwxrwxrwx file
ls -l file
и т.д.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

static const int MAX_PERM_BIT = 10;

void print_mode(mode_t mode) {
	for(int i = MAX_PERM_BIT - 1; i > 0; --i) {
		const mode_t permission = (mode & (1 << (i - 1)));
		//printf(" %04X %04X", 1 << (i - 1), permission);
		switch(permission) {
		case 0:
			printf("-");
			break;
		case S_IRUSR:
			printf("r");
			break;
		case S_IWUSR:
			printf("w");
			break;
		case S_IXUSR:
			printf("x");
			break;
		case S_IRGRP:
			printf("r");
			break;
		case S_IWGRP:
			printf("w");
			break;
		case S_IXGRP:
			printf("x");
			break;
		case S_IROTH:
			printf("r");
			break;
		case S_IWOTH:
			printf("w");
			break;
		case S_IXOTH:
			printf("x");
			break;
		default:
			printf(" UNKNOWN");
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc != 3) {
		printf("usage: %s -rwxrwxrwx file\n", argv[0]);
		return 0;
	}

	const char* permissions = argv[1];
	const char* path = argv[2];
	
	if(strlen(permissions) != MAX_PERM_BIT) {
		printf("wrong permissions string(%s): must be -rwxrwxrwx\n", permissions);
		exit(EXIT_FAILURE);
	}
	printf("permissions %s path %s \n", permissions, path);

	mode_t mode = 0;
	const char allowed[] = "xwr";
	for(int shift = 0; shift < MAX_PERM_BIT - 1; ++shift) {
		const char permission = permissions[MAX_PERM_BIT - shift - 1];
		//printf("permission: %c == %c\n", permission, allowed[shift % 3]);
		if(permission == allowed[shift % 3]) {
			mode |= (1 << shift);
		} else if(permission != '-') {
			fprintf(stderr, "wrong permission value: %c (must be one of {'-', 'r', 'w', 'x'})\n", permission);
			exit(EXIT_FAILURE);
		}
	}
	/*printf("mode: %08X\n", mode);
	print_mode(mode);
	printf("\n");*/

	if(chmod(path, mode)) {
		perror("chmod");
		exit(EXIT_FAILURE);
	}
	return 0;
}
