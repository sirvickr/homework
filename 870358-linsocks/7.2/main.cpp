/*
6.3. send() или write()
Модифицируйте программу из п.6.2 так, чтобы полученные данные отправлялись 
обратно отправителю (в результате должен получиться свой TCP echo-сервер).
*/

#include <iostream>
#include <unistd.h> // close()
#include <fcntl.h> // fcntl()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>           // std::set
#include <algorithm>     // std::max_element()

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

	struct sockaddr_in server_addr;

	// Создаем TCP сокет
	server_sock = socket(domain, SOCK_STREAM, 0);
	if(server_sock < 0){
		perror(NULL); // Печатаем сообщение об ошибке
		exit(1);
	}

	// Переводим созданный сокет в неблокирующий режим
	fcntl(server_sock, F_SETFL, O_NONBLOCK);

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
	// Глубину очереди для входящих соединений устанавливаем равной 5
	listen(server_sock , 5);

	// Набор клиентских соединений
	typedef set<int> clients_t;
	clients_t clients;
	
	while(true) {
		// Заполняем множество сокетов - серверный и все клиенские 
		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(server_sock, &readset);
		for(auto client : clients)
			FD_SET(client, &readset);

		// Задаём таймаут
		timeval timeout;
		timeout.tv_sec = 30;
		timeout.tv_usec = 0;

		// Находим максимальное значение из всех дескрипторов
		int max_fd = max(server_sock, *max_element(clients.begin(), clients.end()));
		// Ждём события в одном из сокетов
		if(select(max_fd + 1, &readset, NULL, NULL, &timeout) <= 0) {
			perror("select");
			exit(3);
		}

		// Определяем тип события и выполняем соответствующие действия
		if(FD_ISSET(server_sock, &readset)) {
			/* Поступил новый запрос на соединение, используем accept. При нормальном 
			завершении, в структуре addr будет лежать полный адрес клиента, установившего 
			соединение, а в переменной size – его фактическая длина. Вызов же вернет дескриптор 
			присоединенного сокета, через который будет происходить общение с клиентом. */
			struct sockaddr_in addr;
			socklen_t size = sizeof(addr);
			int sock = accept(server_sock, (struct sockaddr *) &addr, &size);
			if(sock < 0) {
				perror("accept");
				close(server_sock);
				exit(1);
			}
			// Печатаем сведения о клиенском хосте (getnameinfo() вынесена в отдельную функцию)
			cout << "connection from " << getHostName(&addr, size) << " accepted" << endl;
			// Переводим сокет принятого клиентского соединения в неблокирующий режим
			fcntl(sock, F_SETFL, O_NONBLOCK);
			// Добавляем этот дескриптор в текущий набор соединений
			clients.insert(sock);
		}
		for(clients_t::iterator it = clients.begin(); it != clients.end(); it++) {
			if(FD_ISSET(*it, &readset)) {
				// Поступили данные от клиента, читаем их
				int bytes = recv(*it, data_buf, DataBufSize - 1, 0);
				if(bytes <= 0) {
					// Соединение разорвано (bytes == 0), либо ошибка (bytes < 0), удаляем сокет из множества
					if(bytes == 0)
						cout << "client disconnected" << endl;
					else
						cout << "client connection error" << endl;
					// Закрываем сокет и удаляем его из текущего набора соединений
					shutdown(*it, SHUT_RDWR);
					close(*it);
					clients.erase(*it);
					continue;
				}
				data_buf[bytes] = '\0';
				cout << "got " << bytes << " bytes: " << data_buf << endl;
				// Отправляем данные обратно клиенту
				send(*it, data_buf, bytes + 1, 0);
			}
		}
	}
	close(server_sock);
	cout << "closed" << endl;
	return 0;
}
