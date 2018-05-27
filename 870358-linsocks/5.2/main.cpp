/*
5.2. getnameinfo()
Добавить к программе из п.5.1 вывод адреса удаленного сокета, с которого пришло сообщение
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
		cout << "e.g " << argv[0] << " 2110" << endl;
		return 0;
	}
	
	uint32_t port_number = strtol(argv[1], NULL, 0);

	int sock; // Дескриптор сокета
	char line[1000]; // Массив для принятой и отсылаемой строки
	struct sockaddr_in servaddr, cliaddr; // Структуры для адресов сервера и клиента
	/* Заполняем структуру для адреса сервера: семейство протоколов TCP/IP, сетевой интерфейс - любой, 
	номер порта - из параметра командной строки. Поскольку в структуре содержится дополнительное не
	нужное нам поле, которое должно быть нулевым, перед заполнением обнуляем ее всю */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port_number);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Создаем UDP сокет
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror(NULL); // Печатаем сообщение об ошибке
		exit(1);
	}
	// Настраиваем адрес сокета
	if(bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror(NULL);
		close(sock);
		exit(1);
	}
	// Основной цикл обслуживания
	while(1) {
		// Максимальная длина для хранения ожидаемого адрcpеса клиента
		socklen_t clilen = sizeof(cliaddr);
		/* Ожидаем прихода запроса от клиента и читаем его. Максимально возможная длина датаграммы – 999 символов,
		адрес отправителя помещаем в структуру cliaddr, его реальная длина будет занесена в переменную clilen */
		int n; // Лоличество принятых символов
		if((n = recvfrom(sock, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
			perror(NULL);
			close(sock);
			exit(1);
		}
		line[n] = '\0'; // Клиент нам может и не прислать символ завершения строки
		// Печатаем принятый текст и сведения о клиенском хосте (getnameinfo() вынесена в отдельную функцию)
		cout << line << " (from " << getHostName(&cliaddr, clilen) << ")" << endl;
		// Уходим ожидать новую датаграмму
	}
	return 0;
}
