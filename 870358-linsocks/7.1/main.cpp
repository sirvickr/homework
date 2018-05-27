/*
6.3. send() или write()
Модифицируйте программу из п.6.2 так, чтобы полученные данные отправлялись 
обратно отправителю (в результате должен получиться свой TCP echo-сервер).
*/

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

const size_t DataBufSize = 8192;
char data_buf[DataBufSize];

int main(int argc, char* argv[])
{
	if(argc < 2) {
		cout << "usage: " << argv[0] << " port_number" << endl;
		cout << "e.g " << argv[0] << " 5555" << endl;
		return 0;
	}

	/* Для проверки можно запускать клиентские программы 
	(допустим, мы запустили эту программу с параметром 5555):
	telnet localhost 5555
	или
	../3.5/task localhost 8888 */
	
	uint32_t port_number = strtol(argv[1], NULL, 0);

	int server_sock; // Дескриптор сервернго сокета
	short domain = AF_INET;

	struct sockaddr_in server_addr, client_addr;

	// Создаем TCP сокет
	server_sock = socket(domain, SOCK_STREAM, 0);
	if(server_sock < 0){
		perror(NULL); // Печатаем сообщение об ошибке
		exit(1);
	}

	/* Заполняем структуру для адреса сервера: семейство протоколов TCP/IP, сетевой интерфейс - любой, 
	номер порта - из параметра командной строки. Поскольку в структуре содержится дополнительное не
	нужное нам поле, которое должно быть нулевым, перед заполнением обнуляем ее всю */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = domain;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_number);

	// Настраиваем адрес сокета
	if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("bind");
		close(server_sock);
		exit(1);
	}
	cout << "bound socket" << endl;

	// Переводим созданный TCP-сокет в пассивное (слушающее) состояние. 
	// Глубину очереди для входящих соединений устанавливаем равной 3
	listen(server_sock , 3);

	int client_sock;
	int bytes; // Количество принятых симолов

	// Максимальное число одновременных соединений
	size_t MaxConnCount = 5;
	// Текущее число соединений
	size_t connCount = 0;
	// Основной цикл сервера
	pid_t pids[MaxConnCount];
	for(size_t i = 0; i < MaxConnCount; ++i)
		pids[i] = -1;
	while(true) {
		// В переменную size заносим максимальную длину ожидаемого адреса клиента
		socklen_t size = sizeof(client_addr);
		/* Ожидаем полностью установленного соединения на слушающем сокете. При нормальном 
		завершении, в структуре client_addr будет лежать полный адрес клиента, установившего 
		соединение, а в переменной size – его фактическая длина. Вызов же вернет дескриптор 
		присоединенного сокета, через который будет происходить общение с клиентом. */
		if(connCount < MaxConnCount) {
			client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &size);
			if(client_sock < 0) {
				perror("accept");
				close(server_sock);
				exit(1);
			}
			// Печатаем сведения о клиенском хосте (getnameinfo() вынесена в отдельную функцию)
			cout << "connection[" << connCount << "] from " << getHostName(&client_addr, size) << " accepted" << endl;
			connCount++;
			pid_t pid = fork();
			if(pid == -1) {
				perror("fork");
				close(client_sock);
				close(server_sock);
				exit(1);
			}
			if (pid) {
				//pid_t pid = getpid();
				/* Мы находимся в дочернем порожденном процессе. 
				Потомок наследует все файловые дескрипторы родителя, а значит, и client_sock.
				Теперь таких сокетов два - в каждом процессе по одному */
				
				/* В цикле принимаем информацию от клиента до тех пор, пока не произойдет ошибка 
				(вызов read() вернет отрицательное значение) или клиент не закроет соединение 
				(вызов read() вернет значение 0). В операциях чтения и записи пользуемся дескриптором
				присоединенного сокета, т. е. значением, которое вернул вызов accept(). */
				cout << "[" << pid << "] receiving from client.. " << endl;
				while((bytes = read(client_sock, data_buf, DataBufSize - 1)) > 0) {
					data_buf[bytes] = '\0';
					cout << "[" << pid << "] got: \"" << data_buf << "\"" << endl;
					// Принятые данные отправляем обратно
					if((bytes = write(client_sock, data_buf, bytes + 1)) < 0) {
						perror(NULL);
						close(server_sock);
						close(client_sock);
						exit(1);
					}
				}
				// Если при чтении возникла ошибка – завершаем работу
				if(bytes < 0) {
					perror(NULL);
					close(server_sock);
					close(client_sock);
					exit(1);
				}
				cout << "[" << pid << "] closing" << endl;
				// Закрываем дескриптор присоединенного сокета и уходим ожидать нового соединения
				shutdown(client_sock, SHUT_RDWR);
				close(client_sock );
				cout << "[" << pid << "] done" << endl;
				exit(0);
			} else {
				cout << "[" << pid << "]: accept next" << endl;
			}
			/* В цикле принимаем информацию от клиента до тех пор, пока не произойдет ошибка 
			(вызов read() вернет отрицательное значение) или клиент не закроет соединение 
			(вызов read() вернет значение 0). В операциях чтения и записи пользуемся дескриптором
			присоединенного сокета, т. е. значением, которое вернул вызов accept(). */
			while((bytes = read(client_sock, data_buf, DataBufSize - 1)) > 0) {
				data_buf[bytes] = '\0';
				// Принятые данные отправляем обратно
				if((bytes = write(client_sock, data_buf, bytes + 1)) < 0) {
					perror(NULL);
					close(server_sock);
					close(client_sock);
					exit(1);
				}
			}
			// Если при чтении возникла ошибка – завершаем работу
			if(bytes < 0){
				perror(NULL);
				close(server_sock);
				close(client_sock);
				exit(1);
			}
			// Закрываем дескриптор присоединенного сокета и уходим ожидать нового соединения
			shutdown(client_sock, SHUT_RDWR);
			close(client_sock);
		} else {
			usleep(100000);
		}
	}
	close(server_sock);
	cout << "closed" << endl;
	return 0;
}
