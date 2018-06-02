/*
8.1. Создайте программу, которая будет запускать в дочернем процессе другую программу, путь 
к файлу которой задается аргументом командной строки, а также выводить в стандартный поток вывода 
информацию о статусе завершения этой программы. При реализации должны быть использованы функции 
fork(), execl(), waitpid() и макроопределения WIFEXITED(), WEXITSTATUS(), WIFSIGNALED(), WTERMSIG().
Запуск:
make
./task prog
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
#ifdef ANOTHER_DEMO_PROGRAM
	printf("DAS IST OTHER PROGRAM: %d (argc %d:", getpid(), argc);
	for(int i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf(")\n");
	printf("performing some processing.");
	fflush(stdout);
	for(int i = 0; i < 10; i++) {
		printf(".");
		usleep(500000);
		fflush(stdout);
	}
	printf(" done\n");
	return 259;	
#else
	if(argc != 2) {
		printf("usage: %s file\n", argv[0]);
		return 0;
	}

	char* prog = argv[1];

	printf("the main program process id: %d\n", getpid());
	pid_t child_pid = fork();
	if(child_pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(child_pid > 0) {
		printf("this is the main process with id %d (child pid %d)\n", getpid(), child_pid);
	} else {
		printf("this is the child process with id %d\n", getpid());
		execl(prog, prog, "hi", NULL);
		// возврат из функции произойдет только в случае ошибки
		printf("execl failed\n");
	}
	int status = 0;
	printf("waiting for child process..\n");
	waitpid(child_pid, &status, 0);
	printf("main program finished (child process returned %d):\n", status);
	printf("WIFEXITED\t%d\n", WIFEXITED(status));
	printf("WEXITSTATUS\t%d\n", WEXITSTATUS(status));
	printf("WIFSIGNALED\t%d\n", WIFSIGNALED(status));
	printf("WTERMSIG\t%d\n", WTERMSIG(status));

	return 0;
#endif
}
