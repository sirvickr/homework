/*
10.1. С использованием функции sigaction() назначить обработчик сигнала SIGCHLD. 
В обработчике вызывать для завершившихся процессов функцию из семейства wait(). 
Проверить корректность работы получившейся программы.
Запуск:
make
./task
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

sig_atomic_t count = 0;
pid_t child_pid = 0;

// блокирует приём сигнала процессом
void lock_sig(int signum) {
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, signum);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

// разблокирует приём сигнала процессом
void unlock_sig(int signum) {
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, signum);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

void chld_handler(int signum) {
	// блокируем прерывание обработчика тем же сигналом
	lock_sig(SIGCHLD);

	printf("SIGCHLD: ");
	int status = 0;
	waitpid(child_pid, &status, 0);
	printf("child process finished with %d\n", status);
	++count;

	// перед выходом разблокируем сигнал
	unlock_sig(SIGCHLD);
}

int main(int argc, char* argv[])
{
	struct sigaction sa;
	bzero(&sa, sizeof(sa));
	sa.sa_handler = chld_handler;
	if(sigaction(SIGCHLD, &sa, NULL) != 0) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	printf("the main program process id: %d\n", getpid());
	child_pid = fork();
	if(child_pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(child_pid > 0) {
		printf("this is the main process with id %d (child pid %d)\n", getpid(), child_pid);
	} else {
		printf("this is the child process with id %d, performing some processing\n", getpid());
		fflush(stdout);
		for(int i = 0; i < 10; i++) {
			printf(".");
			usleep(500000);
			fflush(stdout);
		}
		printf(" done\n");
		return 0;
	}
	while(!count) {
		printf("waiting for child process..\n");
		usleep(1500000);
	}
	printf("main process finished\n");

	return 0;
}
