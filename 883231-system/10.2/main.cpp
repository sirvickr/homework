/*
10.2. С помощью функции sigprocmask() заблокировать получение сигнала SIGUSR1, 
после чего восстановить исходную маску сигналов процесса. Проверить корректность 
работы получившейся программы.
Запуск:
make
./task 0
./task 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

sig_atomic_t count = 0;

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

void usr_handler(int signum) {
	// блокируем прерывание обработчика тем же сигналом
	lock_sig(SIGUSR1);

	printf("<SIGUSR1:%d>", count);
	fflush(stdout);
	++count;

	// перед выходом разблокируем сигнал
	unlock_sig(SIGUSR1);
}

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("usage: %s flag(1 - skip one SIGUSR1, 0 - don't skip)\n", argv[0]);
		return 0;
	}
	
	bool skip = !('0' == argv[1][0]);
	
	printf("the main program process id: %d\n", getpid());
	pid_t child_pid = fork();
	if(child_pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(child_pid == 0) {
		printf("this is the child process with id %d, performing some processing.\n", getpid());
		fflush(stdout);

		struct sigaction sa;
		bzero(&sa, sizeof(sa));
		sa.sa_handler = usr_handler;
		if(sigaction(SIGUSR1, &sa, NULL) != 0) {
			perror("sigaction");
			exit(EXIT_FAILURE);
		}

		for(int i = 0; i < 12; i++) {
			printf(".");
			fflush(stdout);
			usleep(500000);
			if(skip) {
				switch(i) {
				case 1:	
					lock_sig(SIGUSR1);
					break;
				case 5:
					unlock_sig(SIGUSR1);
					break;
				}
			}
		}
		printf(" done\n");
		return count;
	}
	
	printf("this is the main process with id %d (child pid %d): waiting for child process..\n", getpid(), child_pid);
	// после паузы посылаем первый сигнал дочернему процессу
	usleep(1500000);
	kill(child_pid, SIGUSR1);
	// после паузы посылаем второй сигнал дочернему процессу
	usleep(1500000);
	kill(child_pid, SIGUSR1);
	// после паузы посылаем третий сигнал дочернему процессу
	usleep(1500000);
	kill(child_pid, SIGUSR1);
	kill(child_pid, SIGUSR1);
	// дожидаемся завершения работы дочернего процесса
	int status = 0;
	waitpid(child_pid, &status, 0);

	printf("\nmain program finished (child process returned %d)\n", status);

	return 0;
}
